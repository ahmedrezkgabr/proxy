#include "MyCallBack.hpp"

void MyCallBack::connection_lost(const std::string &cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void MyCallBack::message_arrived(mqtt::const_message_ptr msg)
{
    std::string topic = msg->get_topic();
    uint8_t index = 255;

    /* carla */
    if (topic == "carla/sensors")
    {
        index = 0;
    }
    else
    {
        /* getting the id of the rpi */
        index = std::stoi(topic.substr(4, 2));
    }
    /* set the corresponding bit */
    recived_msg_flag |= (1 << index);

    /* set the corresponding string */
    this->msg[index] = msg->to_string();

    /* logging information */
    std::cout << "Message arrived on topic '"
              << msg->get_topic() << "': " << msg->to_string() << std::endl;
}

void MyCallBack::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Delivery complete for data: " << token->get_message()->get_payload()
              << ", @ topic: " << token->get_message()->get_topic()
              << std::endl;
}