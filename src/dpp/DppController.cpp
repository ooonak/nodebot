#include "dpp/DppController.hpp"

#include <functional>
#include <iostream>
#include <thread>

using namespace std::placeholders;

ok::DppController::DppController(const ok::Config &config,
                                 std::shared_ptr<ok::NodeQueues> nodeQueues, const std::shared_ptr<spdlog::logger>& logger)
    : mConfig{config}, mLogger{logger}, mNodeQueues{nodeQueues}
{
  if (mLogger == nullptr)
  {
    throw std::invalid_argument("Logger is nullptr");
  }

  mBot = std::make_shared<dpp::cluster>(mConfig.token, dpp::i_default_intents | dpp::i_message_content);

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

  mBot->on_ready(
      [this](const dpp::ready_t event)
      {
        mBot->current_user_get_guilds(
            std::bind(&ok::DppController::onGetGuilds, this, _1));
      });
}

ok::DppController::~DppController() { mLogger->debug(__func__); }

void ok::DppController::onGetGuilds(const dpp::confirmation_callback_t &event)
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
      mLogger->info("I'm a Discord bot named '{}' ({}) connected to guild '{}' ({})", mBot->me.username, mBot->me.id, mGuild->name, mGuild->id);

      mBot->start_timer(std::bind(&ok::DppController::onTimerTick, this), mConfig.updateFrequencySeconds);
    }
  }
}

void ok::DppController::onTimerTick()
{
  if (mStop == true)
  {
    mLogger->info("DppController about to shutdown.");
    mBot->shutdown();
    return;
  }

  switch (mState)
  {
    case State::Init:
      mChannelController = std::make_unique<ok::ChannelController>(
          mBot, mConfig.realm, mConfig.subRealm, mConfig.channelLifetimeInHours, mLogger);
      if (mChannelController != nullptr)
      {
        mChannelController->start(mGuild->id);
        mState = State::WaitingForChannels;
      }
      else
      {
        mLogger->warn("{} Eccor occured, mChannelController == nullptr.", __func__);
        mStop = true;
      }
      break;
    case State::WaitingForChannels:
      if (mChannelController->ready(mChannelId))
      {
        mNodeController = std::make_unique<ok::NodeController>(mBot, mChannelId);
        mState = State::WaitingForThreads;
      }
      else if (mChannelController->errorOccured())
      {
        mLogger->warn("{} Eccor occured (channel controller), we need to stop.", __func__);
        mStop = true;
      }
      break;
    case State::WaitingForThreads:
      if (mNodeController != nullptr)
      {
        if (mNodeController->ready())
        {
          // TODO mSlashCommandController = std::make_unique<NodeBot::SlashCommandController>(mBot, mNodes, mLogger);
          mBot->start_timer(std::bind(&ok::DppController::onMessageTimerTick, this), 1);
          mState = State::Ready;
        }
        else if (mNodeController->errorOccured())
        {
          mLogger->warn("{} Eccor occured (node controller), we need to stop.", __func__);
          mStop = true;
        }
      }
      else
      {
        mLogger->warn("{} Eccor occured, mNodeController == nullptr.", __func__);
        mStop = true;
      }
      break;
    case State::Ready:
      if (mNodeQueues != nullptr && mNodeQueues->changes())
      {
        // TODO Make a copy of all nodes in the queue, KISS for now. Thread-safe to
        // work on local copy.
        mNodes = mNodeQueues->nodes();
        mNodeController->update(mChannelId, mNodes);

        if (mSlashCommandController != nullptr)
        {
          mSlashCommandController->start(mGuild->id);
        }
      }
      break;
  }
}

void ok::DppController::onMessageTimerTick()
{
  dpp::snowflake channelId{0};
  if (mChannelController->ready(channelId))
  {
    while (mNodeQueues != nullptr && mNodeQueues->messages())
    {
      const auto msg = mNodeQueues->popMessage();
      if (msg.first > 0 && msg.first <= mNodes.size())
      {
        if (mNodeController != nullptr)
        {
          const auto threadId = mNodeController->threadId(msg.first);
          if (threadId != dpp::snowflake{})
          {
            mBot->message_create(dpp::message(threadId, msg.second));
          }
        }
      }
    }
  }
}

void ok::DppController::start()
{
  if (mBot != nullptr)
  {
    mLogger->info("Starting DppController in thread {} (Dpp {})", __func__, std::hash<std::thread::id>{}(std::this_thread::get_id()), dpp::utility::version());
    mBot->start(dpp::st_wait);
  }
}

void ok::DppController::stop() { mStop = true; }
