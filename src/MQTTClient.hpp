#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <tuple>
#include <optional>
#include "spdlog/spdlog.h"
#include <mosquittopp.h>
#include "MQTTConfig.hpp"
#include "IngressQueue.hpp"

namespace ok {

class MQTTClient : public mosqpp::mosquittopp {
public:
    explicit MQTTClient(const std::shared_ptr<spdlog::logger> &logger, const ok::MQTTConfig config, IngressQueue* ingressQueue);

    ~MQTTClient();

protected:
    void on_connect(int rc) override;

    void on_disconnect(int rc) override;
    
    void on_message(const struct mosquitto_message * message) override;
	
    void on_subscribe(int mid, int qos_count, const int * granted_qos) override;
	
    void on_unsubscribe(int mid) override;

    bool containsNonAlpha(const std::string& str) const;

    std::optional<ok::IngressMessage> parse(const std::string& topic) const;

private:
    static constexpr int KEEPALIVESECONDS = 120;

    std::shared_ptr<spdlog::logger> logger_{nullptr};
    const MQTTConfig config_{};
    std::string topicBase_;
    IngressQueue* ingressQueue_{nullptr};
};

} // namespace ok
