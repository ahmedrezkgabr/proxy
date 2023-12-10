#include <iostream>
#include <math.h>
#include "config.hpp"
#include "mqtt/async_client.h"
#include "proxy.hpp"

Proxy::Proxy(const ConfigHandler &config) : proxyClient(config.address, config.clientId, config.maxBufMsgs, nullptr)
{
    /* set the call back of message arrival */
    this->proxyClient.mqtt::async_client::set_message_callback([&](mqtt::const_message_ptr msg)
                                                               {
                                                                    /* take the content of the message */
                                                                    std::string topic = msg->get_topic();
                                                                    std::string content = msg->to_string();

                                                                    /* which topic i received on */
                                                                    for(uint8_t i = 0; i < this->numberOfRpis + 1; i++)
                                                                    {
                                                                        if(topic == config.subTopicsNames[i])
                                                                        {
                                                                            /* set the corresponding bit */
                                                                            this->Rx |= (1 << i);

                                                                            /* cpy the content */
                                                                            this->sensorsMsgs[i] = content;
                                                                        }
                                                                    } });

    /* set the call back of connection */
    this->proxyClient.mqtt::async_client::set_connected_handler([&](const std::string &cause)
                                                                { std::cout << "connected to broker @ "
                                                                            << this->proxyClient.get_server_uri()
                                                                            << std::endl; });

    /* set the call back of the connection lost */
    this->proxyClient.mqtt::async_client::set_connection_lost_handler([&](const std::string &cause)
                                                                      { std::cout << "connection to broker @ "
                                                                                  << this->proxyClient.get_server_uri()
                                                                                  << " lost!" << std::endl; });

    /* set the call back of disconnection */
    this->proxyClient.mqtt::async_client::set_disconnected_handler([&](const mqtt::properties &, mqtt::ReasonCode)
                                                                   { std::cout << "disconnected to broker @ "
                                                                               << this->proxyClient.get_server_uri()
                                                                               << std::endl; });

    /* set the number of rpis */
    this->numberOfRpis = config.numberOfRpis;

    /* resize the messages data vectors */
    this->actionsMsgs.resize(this->numberOfRpis + 1);
    this->sensorsMsgs.resize(this->numberOfRpis + 1);

    /* calculate the mask of the Rx flag */
    this->maskRx = std::pow(2, this->numberOfRpis + 1) - 2;

    /* create the topics of publishing and subscription */
    for (uint8_t i = 0; i < this->numberOfRpis + 1; i++)
    {
        this->pubTopics.push_back({proxyClient, config.pubTopicsNames[i], config.qualityOfService, config.retainedFlag});
        this->subTopics.push_back({proxyClient, config.subTopicsNames[i], config.qualityOfService, config.retainedFlag});
    }

    /* create a connection options handler */
    this->connectionOptions = mqtt::connect_options_builder()
                                  .keep_alive_interval(std::chrono::seconds(config.keepAliveTime))
                                  .clean_session(config.cleanSession)
                                  .automatic_reconnect(config.autoReconnect)
                                  .finalize();
}
