#ifndef RPI_HPP
#define RPI_HPP
#include "mqtt/async_client.h"
class MyCallBack : public virtual mqtt::callback
{
public:
    int recived_msg_flag = 0;
    std::string msg;
    uint64_t counter_rec = 0;
    uint64_t counter_sen = 0;
    /* overriding the functions on every action may happen */
    void connection_lost(const std::string &cause) override;         /* call back function called when lossing the connection with the broker */
    void message_arrived(mqtt::const_message_ptr msg) override;      /* call back function called when receiving a message */
    void delivery_complete(mqtt::delivery_token_ptr token) override; /* call back function called when delivery complete */
};

#endif