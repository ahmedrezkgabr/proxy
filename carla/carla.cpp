#include "carla.hpp"

void MyCallBack::connection_lost(const std::string &cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void MyCallBack::message_arrived(mqtt::const_message_ptr msg)
{
    this->counter_received++;
    recived_msg_flag = 1;
    std::cout << "Message arrived on topic '" << msg->get_topic() << "': " << msg->to_string()
              << this->counter_received << ",,, "
              << this->counter_sent << std::endl;
    this->msg = msg->to_string();
}

void MyCallBack::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Delivery complete for data: " << token->get_message()->get_payload()
              << ", @ topic: " << token->get_message()->get_topic()
              << std::endl;
}