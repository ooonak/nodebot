#pragma once

#include <string>
#include "spdlog/spdlog.h"
#include <mosquittopp.h>

namespace ok {

class MQTTClient : public mosqpp::mosquittopp {
public:
    explicit MQTTClient(const std::shared_ptr<spdlog::logger> &logger);

    ~MQTTClient();

    void on_connect(int rc) override;

    void on_disconnect(int rc) override;
    
    void on_message(const struct mosquitto_message * message) override;
	
    void on_subscribe(int mid, int qos_count, const int * granted_qos) override;
	
    void on_unsubscribe(int mid) override;
	
    void on_log(int level, const char * str) override;
	
    void on_error() override;

private:
    std::shared_ptr<spdlog::logger> logger_;

};

} // namespace ok