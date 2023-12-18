#include "MQTTClient.hpp"

ok::MQTTClient::MQTTClient(const std::shared_ptr<spdlog::logger> &logger, const MQTTConfig config, MessageQueue* ingressQueue)
    : mosquittopp(config.id.c_str()), logger_{logger}, config_{std::move(config)}, ingressQueue_{ingressQueue}
{
  if (logger_ == nullptr)
  {
    throw std::runtime_error("No logger (nullptr)");
  }

  if (containsNonAlpha(config_.base) || config_.base.empty())
  {
    throw std::runtime_error("Only non-empty lowercase ASCII string (a-z) is accepted in topic.");
  }

  topicBase_.append("/");
  topicBase_.append(config_.base);
  topicBase_.append("/+/+/");

  logger_->info("Trying to connect to {}:{}", config_.host, config_.port);
  connect(config_.host.c_str(), config_.port, KEEPALIVESECONDS);
}

ok::MQTTClient::~MQTTClient() {}

bool ok::MQTTClient::sendMessage(const ok::Message &msg)
{
  if (!connected_)
  {
    return false;
  }

  const auto topic = topicBase_ + "/" + std::to_string(msg.id) + "/" + ok::toString(msg.action); 
  const auto rc = publish(nullptr, topic.c_str(), msg.jsonPayload.length(), msg.jsonPayload.c_str());
  if (rc != MOSQ_ERR_SUCCESS)
  {
    logger_->error("Failed to publish {} {}", rc, strerror(rc));
    return false;
  }

  return true;
}

void ok::MQTTClient::on_connect(int rc)
{
  if (rc == MOSQ_ERR_SUCCESS)
  {
    connected_ = true;

    logger_->info("Connected");

    const std::vector<std::string> actionsStr{"connect", "disconnect", "message", "registercommand",
                                              "unregistercommand"};
    for (const auto &str : actionsStr)
    {
      if (!containsNonAlpha(str))
      {
        const auto topic = topicBase_ + str;
        logger_->info("Subscribing to {}", topic);
        subscribe(nullptr, topic.c_str());
      }
    }
  }
  else
  {
    logger_->error("Failed to connect {} {}", rc, strerror(rc));
    // TODO: propably just reconnect...
  }
}

void ok::MQTTClient::on_disconnect(int rc) 
{
  connected_ = false; 
  logger_->info("Disconnected {} {}", rc, strerror(rc));
  reconnect();
}

void ok::MQTTClient::on_message(const struct mosquitto_message *message)
{
  if (ingressQueue_ != nullptr)
  {
    if (auto msg = parse(message->topic, logger_))
    {
      (*msg).jsonPayload = std::string(reinterpret_cast<const char *>(message->payload), message->payloadlen);
      ingressQueue_->push(std::move(*msg));
    }
  }
}

void ok::MQTTClient::on_subscribe(int /*mid*/, int /*qos_count*/, const int * /*granted_qos*/)
{
  logger_->debug("Subscribed");
}

void ok::MQTTClient::on_unsubscribe(int /*mid*/) { logger_->debug("Unsubscribed"); }

bool ok::MQTTClient::containsNonAlpha(const std::string &str)
{
  return str.find_last_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;
}

std::optional<ok::Message> ok::MQTTClient::parse(const std::string &topic,
                                                        const std::shared_ptr<spdlog::logger> &logger)
{
  try
  {
    ok::Message msg{};

    size_t count = 0;
    size_t last = 0;
    size_t next = 0;

    while ((next = topic.find('/', last)) != std::string::npos)
    {
      if (count == 2)
      {
        msg.group = topic.substr(last, next - last);
      }
      else if (count == 3)
      {
        msg.id = std::stoull(topic.substr(last, next - last));
      }

      last = next + 1;
      count++;
    }

    msg.action = ok::fromString(topic.substr(last));

    if (msg.id > 0 && msg.action != Message::ActionT::None && !msg.group.empty())
    {
      return msg;
    }
  }
  catch (const std::exception &exc)
  {
    if (logger != nullptr)
    {
      logger->error("{} {}", __func__, exc.what());
    }
  }

  return std::nullopt;
}
