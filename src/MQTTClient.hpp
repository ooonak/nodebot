#pragma once

#include <mosquittopp.h>

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>
#include <atomic>
#include "MessageQueue.hpp"
#include "MQTTConfig.hpp"
#include "spdlog/spdlog.h"

namespace ok
{

class MQTTClient : public mosqpp::mosquittopp
{
 public:
  explicit MQTTClient(const std::shared_ptr<spdlog::logger>& logger, const ok::MQTTConfig config, MessageQueue* ingressQueue);

  ~MQTTClient();

  bool sendMessage(const ok::Message &msg);

  static bool containsNonAlpha(const std::string& str);

  static std::optional<ok::Message> parse(const std::string& topic,
                                                 const std::shared_ptr<spdlog::logger>& logger);

 protected:
  void on_connect(int rc) override;

  void on_disconnect(int rc) override;

  void on_message(const struct mosquitto_message* message) override;

  void on_subscribe(int mid, int qos_count, const int* granted_qos) override;

  void on_unsubscribe(int mid) override;

 private:
  static constexpr int KEEPALIVESECONDS = 120;
  std::atomic<bool> connected_{false};
  std::shared_ptr<spdlog::logger> logger_{nullptr};
  const MQTTConfig config_{};
  std::string topicBase_;
  MessageQueue* ingressQueue_{nullptr};
};

}  // namespace ok
