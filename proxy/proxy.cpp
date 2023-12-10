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

Proxy::~Proxy()
{
    this->actionsMsgs.~vector();
    this->sensorsMsgs.~vector();
    this->pubTopics.~vector();
    this->subTopics.~vector();
}

/**
 * @brief Connects to an MQTT server using the default options.
 * @return token used to track and wait for the connect to complete. The
 *  	   token will be passed to any callback that has been set.
 * @throw exception for non security related problems
 * @throw security_exception for security related problems
 */
void Proxy::connect(void)
{
    try
    {
        /* connect the client to the broker with the connection options that have been set */
        this->proxyClient.connect(this->connectionOptions)->wait();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

/**
 * @brief Disconnects from the MQTT broker.
 * @return token used to track and wait for the disconnect to complete.
 *  	   The token will be passed to any callback that has been set.
 * @throw exception for problems encountered while disconnecting
 */
void Proxy::disconnect(void)
{
    try
    {
        /* disconnect the client from the broker */
        this->proxyClient.mqtt::async_client::disconnect()->wait();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

/**
 * @brief Returns the address of the server used by this client.
 * @return The server's address, as a URI String.
 */
std::string Proxy::get_server_uri(void)
{
    /* return the servers's address */
    return this->proxyClient.mqtt::async_client::get_server_uri();
}

void Proxy::subscribe(void)
{
    /* loop and subscribe with each topic of subscribtion */
    for (uint8_t i = 0; i < this->numberOfRpis + 1; i++)
    {
        this->subTopics[i].mqtt::topic::subscribe();
    }
}
Proxy_Flag_t Proxy::getRxFalg()
{
    /* if bit-0 of the flag equal to 1 so the proxy has received from carla*/
    if (this->Rx & 1)
        return Proxy_Flag_t::CARLA;

    /* if bits[1:numberOfRpis] all equal to 1 so the proxy has received from all rpis */
    else if ((this->Rx == this->maskRx) || (this->Rx == this->maskRx + 1))
        return Proxy_Flag_t::RPIS;

    else
        return Proxy_Flag_t::NOT;
}
void Proxy::publish(Proxy_Flag_t type)
{
    /* publish what has been received from carla */
    if (type == Proxy_Flag_t::CARLA)
        for (uint8_t i = 0; i < this->numberOfRpis; i++)
            this->pubTopics[i + 1].mqtt::topic::publish(this->sensorsMsgs[i + 1]);

    /* publish what has been received from rpis */
    else if (type == Proxy_Flag_t::RPIS)
        this->pubTopics[0].mqtt::topic::publish(this->actionsMsgs[0]);
}

void Proxy::clearRxFlag(Proxy_Flag_t type)
{
    /* clear the corresponding bit of carla (bit-0) */
    if (type == Proxy_Flag_t::CARLA)
        this->Rx &= (~(1));
    
    /* clear the corresponding bits of rpis bits[1:numberOfRpis] */
    else
        this->Rx &= (~(this->maskRx));
}

/* not yet */
void Proxy::parse()
{
    this->sensorsMsgs[1] = this->sensorsMsgs[0];
    this->sensorsMsgs[2] = this->sensorsMsgs[0];
}

/* not yet */
void Proxy::compose()
{
    this->actionsMsgs[0] = this->actionsMsgs[1] + "," + this->actionsMsgs[2];
}