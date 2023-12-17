#include "MQTTClient.hpp"

ok::MQTTClient::MQTTClient(const std::shared_ptr<spdlog::logger> &logger, const MQTTConfig config, IngressQueue* ingressQueue)
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

ok::MQTTClient::~MQTTClient()
{
}

void ok::MQTTClient::on_connect(int rc)
{
    if (rc == MOSQ_ERR_SUCCESS)
    {
      logger_->info("Connected");

      const std::vector<std::string> actionsStr { "connect", "disconnect", "message", "register_command", "unregister_command" };
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

void ok::MQTTClient::on_disconnect(int /*rc*/)
{
    logger_->info("Disconnected");
}
    
void ok::MQTTClient::on_message(const struct mosquitto_message * message)
{
    if (ingressQueue_ != nullptr)
    {
        if (auto msg = parse(message->topic))
        {
            (*msg).jsonPayload = std::string(reinterpret_cast<const char*>(message->payload), message->payloadlen);
          //logger_->debug("Received MQTT message: node id: {}, type: {}, group: {}, subgroup: {}, payload: {}", std::get<0>(*info), std::get<2>(*info), payload);
          ingressQueue_->push(*msg);
        }
    }
}

void ok::MQTTClient::on_subscribe(int /*mid*/, int /*qos_count*/, const int * /*granted_qos*/)
{
    logger_->info("Subscribed");
}
	
void ok::MQTTClient::on_unsubscribe(int /*mid*/)
{
    logger_->info("Unsubscribed");
}

bool ok::MQTTClient::containsNonAlpha(const std::string& str) const
{
    return str.find_last_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;
}

std::optional<ok::IngressMessage> ok::MQTTClient::parse(const std::string& topic) const
{
    try {
        ok::IngressMessage msg{};

        size_t count = 0;
        size_t last = 0;
        size_t next = 0;

        while ((next = topic.find('/', last)) != std::string::npos)
        {
            if (count == 1)
            {
                msg.group = topic.substr(last, next-last);
            }
            else if (count == 2)
            {
                msg.subGroup = topic.substr(last, next-last);
            }
            else if (count == 3)
            {
                msg.id = std::stoull(topic.substr(last, next-last));
            }

            last = next+1;
            count++;
        }

        msg.action = ok::fromString(topic.substr(last));

        if (msg.id > 0 && msg.action != ActionT::None && !msg.group.empty() && !msg.subGroup.empty())
        {
            return msg;
        }
    }
    catch (const std::exception &exc)
    {
        logger_->error("{} {}", __func__, exc.what());
    }

    return std::nullopt;
}
