#include "NodeController.hpp"

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
}

void nb::NodeController::update(uint64_t id, const std::string &jsonStr, dpp::snowflake channelId)
{
  if (mNodes.find(id) == mNodes.end())
  {
    //mNodes[id] = std::pair<dpp::snowflake{0}, jsonStr>;

    dpp::embed embed = dpp::embed().
                       set_color(dpp::colors::sti_blue).
                       set_title("Some name").
                       set_description("Some description here").
                       add_field(
                               "Regular field title",
                               "Some value here"
                           );
    mBot->message_create(dpp::message(channelId, embed));
  }
}

void nb::NodeController::onMessageCreate(const dpp::confirmation_callback_t &event)
{
  if (event.is_error())
  {
    const auto err = event.get_error();
    mLogger->error("{} {} {}", __func__, err.code, err.message);
  }
  else
  {
    const auto msg = std::get<dpp::message>(event.value);
    spdlog::info("Created message {} {}", msg.id, msg.content);
  }
}
