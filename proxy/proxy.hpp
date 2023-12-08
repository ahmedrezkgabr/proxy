#ifndef PROXY__HPP_
#define PROXY__HPP_

#include <iostream>
#include "mqtt/async_client.h"

enum En_Flag_t
{
    CARLA,
    RPIS,
    ACTIONS,
    SENSORS
};

class Proxy : private mqtt::async_client
{
private:
    std::string configFilePath;

public:
    Proxy();
    ~Proxy();
    void setConfigFilePath(std::string);
    bool loadConfiguaration();
    void loadDefaultConfiguaration();
    std::string getConfigFilePath();
    mqtt::token_ptr subscribe(void);
    mqtt::delivery_token_ptr publish(std::string);
    uint8_t getRxFalg();
};

#endif /* PROXY__HPP_ */