#include "NodeController.hpp"

using namespace std::placeholders;

static std::string Needle{"ID: "};

nb::NodeController::NodeController(std::shared_ptr<dpp::cluster> bot)
    : mBot{bot}, mLogger{spdlog::get("DPP")}
{
  mBot->on_message_create(std::bind(&NodeController::onMessageCreate, this, _1));
  mBot->on_message_update(std::bind(&NodeController::onMessageUpdate, this, _1));
}

void nb::NodeController::update(uint64_t id, const std::string &jsonStr, dpp::snowflake channelId)
{
  if (mNodes.find(id) == mNodes.end())
  {
    mNodes[id] = dpp::message{0};

    // TODO Remove test...
    dpp::embed embed = dpp::embed().
                       set_color(dpp::colors::sti_blue).
                       set_title("Some Node Name").
                       set_description("Some description here").
                       add_field("Regular field title", "Some value here").
                       set_footer(Needle + std::to_string(id), "");

    mBot->message_create(dpp::message(channelId, embed));
  }
  else
  {
    dpp::embed updatedEmbed = mNodes[id].embeds[0];

    if (updatedEmbed.color == dpp::colors::sti_blue)
    {
      updatedEmbed.set_color(dpp::colors::red);
      updatedEmbed.set_title("42");
    }
    else
    {
      updatedEmbed.set_color(dpp::colors::sti_blue);
      updatedEmbed.set_title("!42");
    }

    mNodes[id].embeds[0] = updatedEmbed;
    mBot->message_edit(mNodes[id]);
  }
}

void nb::NodeController::onMessageCreate(const dpp::message_create_t &event)
{
  for (const auto embed : event.msg.embeds)
  {
    if (const auto footer = embed.footer)
    {
      const auto idPos = (*footer).text.find(Needle);
      if (idPos != std::string::npos)
      {
        const auto idStr = (*footer).text.substr(idPos + Needle.length());
        const uint64_t id = std::stoull(idStr, nullptr, 10);
        if (id != 0 && (mNodes.find(id) != mNodes.end()))
        {
          mNodes[id] = event.msg;
          mLogger->info("Message created, mapped message id {} -> snowflake {}", id, mNodes[id].id);
        }
      }
    }
  }
}

void nb::NodeController::onMessageUpdate(const dpp::message_update_t &event)
{
  mLogger->debug(__func__);
}
