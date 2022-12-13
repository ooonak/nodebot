#include "dpp/DppController.hpp"

#include <functional>
#include <iostream>
#include <thread>

using namespace std::placeholders;

nb::DppController::DppController(const nb::Config &config,
                                 std::shared_ptr<nb::NodeQueues> nodeQueues)
    : mConfig{config}, mLogger{spdlog::get("DPP")}, mNodeQueues{nodeQueues}
{
  mBot = std::make_shared<dpp::cluster>(mConfig.token);

  // Direct logs to spd
  mBot->on_log(
      [this](const dpp::log_t &event)
      {
        switch (event.severity)
        {
          case dpp::ll_trace:
            mLogger->trace("{}", event.message);
            break;
          case dpp::ll_debug:
            mLogger->debug("{}", event.message);
            break;
          case dpp::ll_info:
            mLogger->info("{}", event.message);
            break;
          case dpp::ll_warning:
            mLogger->warn("{}", event.message);
            break;
          case dpp::ll_error:
            mLogger->error("{}", event.message);
            break;
          case dpp::ll_critical:
          default:
            mLogger->critical("{}", event.message);
            break;
        }
      });

  mChannelController = std::make_unique<nb::ChannelController>(
      mBot, mConfig.channelPrefix, mConfig.channelLifetimeInHours);
  mNodeController = std::make_unique<nb::NodeController>(
      mBot, config.nodeName, config.nodeDescription);
  mSlashCommandController =
      std::make_unique<nb::SlashCommandController>(mBot, mNodes);

  mBot->on_ready(
      [this](const dpp::ready_t event)
      {
        mBot->current_user_get_guilds(
            std::bind(&nb::DppController::onGetGuilds, this, _1));
      });
}

nb::DppController::~DppController() { mLogger->debug(__func__); }

void nb::DppController::onGetGuilds(const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    auto guilds = std::get<dpp::guild_map>(event.value);
    for (auto const &[key, value] : guilds)
    {
      mGuild = std::make_unique<dpp::guild>(value);
      mLogger->info(
          "I'm a Discord bot named '{}' ({}) connected to guild '{}' ({})",
          mBot->me.username, mBot->me.id, mGuild->name, mGuild->id);
      mChannelController->start(mGuild->id);
      mBot->start_timer(std::bind(&nb::DppController::onTimerTick, this),
                        mConfig.updateFrequencySeconds);
      mBot->start_timer(std::bind(&nb::DppController::onMessageTimerTick, this),
                        1);
    }
  }
}

void nb::DppController::onTimerTick()
{
  if (mStop == true)
  {
    mLogger->info("DppController about to shutdown.");
    mBot->shutdown();
    return;
  }

  if (mChannelController != nullptr)
  {
    dpp::snowflake channelId{0};
    if (mChannelController->ready(channelId))
    {
      if (mNodeController != nullptr)
      {
        if (mNodeQueues != nullptr && mNodeQueues->changes())
        {
          // Make a copy of all nodes in the queue, KISS for now. Thread-safe to
          // work on local copy.
          mNodes = mNodeQueues->nodes();
          mNodeController->update(channelId, mNodes);

          if (mSlashCommandController != nullptr)
          {
            mSlashCommandController->start(mGuild->id);
          }
        }
      }
    }
  }
}

void nb::DppController::onMessageTimerTick()
{
  dpp::snowflake channelId{0};
  if (mChannelController->ready(channelId));
  {
    while (mNodeQueues != nullptr && mNodeQueues->messages())
    {
      const auto msg = mNodeQueues->popMessage();
      if (msg.first > 0 && msg.first <= mNodes.size())
      {
        std::string str = "[ID: ";
        str.append(std::to_string(msg.first));
        str.append(" ");
        str.append(mNodes.at(msg.first-1).info.name);
        str.append("] ");
        str.append(msg.second);
        mBot->message_create(dpp::message(channelId, str));
      }
    }
  }
}

void nb::DppController::start()
{
  if (mBot != nullptr)
  {
    mLogger->info("Starting DppController in thread {} (Dpp {})", __func__,
                  std::hash<std::thread::id>{}(std::this_thread::get_id()),
                  dpp::utility::version());
    mBot->start(dpp::st_wait);
  }
}

void nb::DppController::stop() { mStop = true; }
