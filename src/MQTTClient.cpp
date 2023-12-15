#include "MQTTClient.hpp"

ok::MQTTClient::MQTTClient(const std::shared_ptr<spdlog::logger> &logger)
: logger_{logger}
 {
    if (logger_ == nullptr)
    {
        throw std::runtime_error("No logger (nullptr)");
    }
}

ok::MQTTClient::~MQTTClient()
{
}

void ok::MQTTClient::on_connect(int /*rc*/)
{
    logger_->info("Connected");
}

void ok::MQTTClient::on_disconnect(int /*rc*/)
{
    logger_->info("Disconnected");
}
    
void ok::MQTTClient::on_message(const struct mosquitto_message * message)
{
    logger_->info("Message {}", message->topic);
}
	
void ok::MQTTClient::on_subscribe(int /*mid*/, int /*qos_count*/, const int * /*granted_qos*/)
{
    logger_->info("Subscribed");
}
	
void ok::MQTTClient::on_unsubscribe(int /*mid*/)
{
    logger_->info("Unsubscribed");
}
	
void ok::MQTTClient::on_log(int /*level*/, const char * /*str*/)
{

}
	
void ok::MQTTClient::on_error()
{

}
