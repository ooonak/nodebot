#include "WebHookController.hpp"

using namespace std::placeholders;

nb::WebHookController::WebHookController(
    std::shared_ptr<dpp::cluster> bot, nb::NodeHandlesT &nodes,
    std::shared_ptr<nb::NodeQueues> nodeQueues)
    : mBot{bot},
      mNodes{nodes},
      mNodeQueues{nodeQueues},
      mLogger{spdlog::get("DPP")}
{
}

void nb::WebHookController::createWebHook(dpp::snowflake guildId,
                                          dpp::snowflake channelId,
                                          uint64_t nodeId)
{
  if (nodeId > 0 && nodeId <= mNodes.size() &&
      mNodes.at(nodeId - 1).webHookUrl.empty())
  {
    if (mNodeQueues != nullptr)
    {
      mNodeQueues->setWebHookUrl(nodeId, "<in-progress>");
    }
    mNodes.at(nodeId - 1).webHookUrl = "<in-progress>";
    auto webhook = dpp::webhook();
    webhook.name = "WebHookNodeId_" + std::to_string(nodeId);
    webhook.guild_id = guildId;
    webhook.channel_id = channelId;

    mBot->create_webhook(
        webhook, std::bind(&WebHookController::onCreateWebHook, this, _1));
  }
}

void nb::WebHookController::onCreateWebHook(
    const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    auto hook = std::get<dpp::webhook>(event.value);
    std::size_t pos = hook.name.find_last_of("_");
    if (pos != std::string::npos)
    {
      try
      {
        uint64_t id = std::stoul(hook.name.substr(pos + 1), nullptr, 10);
        if (id > 0 && id <= mNodes.size())
        {
          mLogger->info("Created WebHook for node {}: {} {}.", id, hook.id,
                        hook.name);
          std::string url = "https://discord.com/api/webhooks/" +
                            std::to_string(hook.id) + "/" + hook.token;
          if (mNodeQueues != nullptr)
          {
            mNodeQueues->setWebHookUrl(id, url);
          }
          mNodes.at(id - 1).webHookUrl = url;
        }
      }
      catch (const std::exception &exc)
      {
        mLogger->error("Could not get id from WebHook name {}, ", hook.name,
                       exc.what());
      }
    }
  }
}
