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
    RPIS
};

class Proxy : private mqtt::async_client
{
private:
    /* client */
    std::string address;
    std::string clientId;
    uint32_t maxBufMsgs;
    bool cleanSession;
    bool autoReconnect;
    std::chrono::seconds keepAliveTime;
    mqtt::connect_options_builder connectionOptions;

    /* topics */
    std::vector<mqtt::topic> pubTopics;
    std::vector<mqtt::topic> subTopics;
    uint8_t QualityOfService;
    bool retainedFlag;

    /* data */
    std::vector<std::string> sensorsMsgs;
    std::vector<std::string> actionsMsgs;

    std::string configFilePath{"./.config"};
    Proxy_Flag_t Rx;

public:
    /**
     * @brief Create a Proxy that can be used to communicate with an MQTT server.
     * @throw exception if an argument is invalid
     */
    Proxy();

    /**
     * @brief Destructor
     */
    ~Proxy();

    /**
     * @brief Sets the configuration File Path.
     *
     * @param filePath as string
     * @return An enum of Proxy_Error_t.
     */
    Proxy_Error_t setConfigFilePath(std::string);

    /**
     * @brief Gets the configuration File Path.
     *
     * @return std::string as the file path.
     */
    std::string getConfigFilePath(void);

    /**
     * @brief Loads the Configuaration.
     *
     * Loads the Configuaration parameters from the configuation file
     * These configuartion ()
     *
     * @return An enum of Proxy_Error_t.
     */
    Proxy_Error_t loadConfiguaration(void);

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
    mqtt::token_ptr connect(void);

    /**
     * @brief Disconnects from the MQTT broker.
     * @return token used to track and wait for the disconnect to complete.
     *  	   The token will be passed to any callback that has been set.
     * @throw exception for problems encountered while disconnecting
     */
    mqtt::token_ptr disconnect(void);

    /**
     * @brief Returns the address of the server used by this client.
     * @return The server's address, as a URI String.
     */
    std::string get_server_uri(void);

    mqtt::token_ptr subscribe(void);

    /**
     * @brief Publishes a messages to topics on the server
     * @param type the kind of publishing (carla or rpis)
     * @return token used to track and wait for the publish to complete. The
     *  	   token will be passed to callback methods if set.
     */
    mqtt::delivery_token_ptr publish(Proxy_Flag_t type);
};

#endif /* PROXY__HPP_ */