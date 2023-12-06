#include "MyCallBack.hpp"

void MyCallBack::connection_lost(const std::string &cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void MyCallBack::message_arrived(mqtt::const_message_ptr msg)
{
    std::string topic = msg->get_topic();

    if (topic == "carla/sensors")
    {
        recived_msg_flag |= 1;
        this->msg_sensors = msg->to_string();
    }
    else if (topic == "rpi/01/actions")
    {
        
        recived_msg_flag |= 2;
        this->msg_actions[0]=msg->to_string();
    }
    else if(topic == "rpi/02/actions")
    {
        recived_msg_flag |= 4;
        this->msg_actions[1]=msg->to_string();
    }
    std::cout << "Message arrived on topic '"
              << msg->get_topic() << "': " << msg->to_string() << std::endl;
}

void MyCallBack::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Delivery complete for data: " << token->get_message()->get_payload()
              << ", @ topic: " << token->get_message()->get_topic()
              << std::endl;
}