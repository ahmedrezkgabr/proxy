#ifndef PROXY__HPP_
#define PROXY__HPP_

#include <iostream>
#include "mqtt/async_client.h"

enum class Proxy_Error_t
{
    OK,
    NOT_OK
};

enum class Proxy_Flag_t
{
    CARLA,
    RPIS,
    NOT
};

class Proxy
{
public:
    /**
     * @brief Create a Proxy that can be used to communicate with an MQTT server.
     * @throw exception if an argument is invalid
     */
    Proxy() = delete;

    Proxy(ConfigHandler &);

    /**
     * @brief Destructor
     */
    ~Proxy();

    /**
     * @brief Gets Receive Flag
     * @return flag used to indicate the state of receive.
     */
    Proxy_Flag_t getRxFalg(void);

    /**
     * @brief Connects to an MQTT server using the default options.
     * @return token used to track and wait for the connect to complete. The
     *  	   token will be passed to any callback that has been set.
     * @throw exception for non security related problems
     * @throw security_exception for security related problems
     */
    void connect(void);

    /**
     * @brief Disconnects from the MQTT broker.
     * @return token used to track and wait for the disconnect to complete.
     *  	   The token will be passed to any callback that has been set.
     * @throw exception for problems encountered while disconnecting
     */
    void disconnect(void);

    /**
     * @brief Returns the address of the server used by this client.
     * @return The server's address, as a URI String.
     */
    std::string get_server_uri(void);

    void subscribe(void);

    /**
     * @brief Publishes a messages to topics on the server
     * @param type the kind of publishing (carla or rpis)
     * @return token used to track and wait for the publish to complete. The
     *  	   token will be passed to callback methods if set.
     */
    void publish(Proxy_Flag_t type);

    void parse();
    void compose();

    void clearRxFlag(Proxy_Flag_t type);

private:
    /* mqtt stuff */
    mqtt::async_client proxyClient;
    std::vector<mqtt::topic> pubTopics;
    std::vector<mqtt::topic> subTopics;

    /* in a struct */
    /* data */
    std::vector<std::string> sensorsMsgs;
    std::vector<std::string> actionsMsgs;
    mqtt::connect_options connectionOptions;
    uint64_t Rx{0};
    uint64_t maskRx{0};
    uint8_t numberOfRpis{0};
    std::mutex flagMutex;
    std::mutex sensorsMutex;
    std::mutex actionsMutex;

    std::vector<std::string> parseJSONString(const std::string &jsonString);
    std::string composeJSONString(const std::vector<std::string> &stringList);
};

#endif /* PROXY__HPP_ */