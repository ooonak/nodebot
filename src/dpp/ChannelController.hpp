#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "dpp/dpp.h"
#include "spdlog/spdlog.h"

namespace ok
{

/**
 * @brief The ChannelController creates a category named as 'realm' from config in guild if non-existent.
 *        Creates a new channel named 'subrealm' from config appended with time of creation.
 *        Deletes all channels that are expired.
 */
class ChannelController
{
 public:
  explicit ChannelController(std::shared_ptr<dpp::cluster> bot, std::string realm, std::string subRealm,
                             int channelLifetimeInHours, const std::shared_ptr<spdlog::logger>& logger);

  /**
   * @brief start: Start processing on guild id.
   * @param guildId
   */
  void start(dpp::snowflake guildId);

  /**
   * @brief ready: Polled by DppController to signal when ready.
   * @param id
   * @return
   */
  bool ready(dpp::snowflake &id) const;

  bool errorOccured() const;

 private:
  std::shared_ptr<dpp::cluster> mBot;
  std::shared_ptr<spdlog::logger> mLogger;

  const std::string mRealm;
  const std::string mSubRealm;
  const int mLifetimeHours;

  std::unique_ptr<dpp::channel> mCategory;
  std::unique_ptr<dpp::channel> mChannel;
  std::vector<dpp::channel> mExpiredChannelsToDelete;
  size_t mChannelsDeleted{0};

  dpp::snowflake mGuildId{0};

  bool mReady{false};
  bool mErrorOccured{false};

  /**
   * @brief onCategorysGet: Callback for get all categorys from guild (Discord server).
   * @param event
   */
  void onCategorysGet(const dpp::confirmation_callback_t &event);

  /**
   * @brief onChannelsGet: Callback for get all channels under category.
   * @param event
   */
  void onChannelsGet(const dpp::confirmation_callback_t &event);

  /**
   * @brief onCategoryCreate: When category was created.
   * @param event
   */
  void onCategoryCreate(const dpp::confirmation_callback_t &event);

  /**
   * @brief onChannelCreate: When a channel was created.
   * @param event
   */
  void onChannelCreate(const dpp::confirmation_callback_t &event);

  /**
   * @brief onChannelDelete: When a channel was deleted.
   * @param event
   */
  void onChannelDelete(const dpp::confirmation_callback_t &event);


  // Helpers
  static std::string ISO8601UTC(dpp::snowflake id);

  bool channelOlderThan(const dpp::channel &channel, int channelLifetimeInHours);

  static bool channelCreatedAfter(const dpp::channel &channel, const dpp::channel &compareAgainst);
};

}  // namespace NodeBot
