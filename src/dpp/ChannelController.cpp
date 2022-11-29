#include "ChannelController.hpp"

using namespace std::placeholders;

nb::ChannelController::ChannelController(std::shared_ptr<dpp::cluster> bot, std::string channelPrefix, int channelLifetimeInHours)
    : mBot{bot}, mLogger{spdlog::get("DPP")}, mPrefix{channelPrefix}, mLifetimeHours{channelLifetimeInHours}
{

}

void nb::ChannelController::start(dpp::snowflake guildId)
{
  mBot->channels_get(guildId, std::bind(&ChannelController::onChannelsGet, this, _1));
}

void nb::ChannelController::onChannelsGet(const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    auto channels = std::get<dpp::channel_map>(event.value);
    for (auto const &[key, value] : channels)
    {
      if (value.get_type() == dpp::CHANNEL_TEXT && value.name.starts_with(mPrefix))
        mLogger->info("Channels '{}' ({})", value.name, value.id);
        mActiveChannel = std::make_unique<dpp::channel>(value);
    }
  }
}
