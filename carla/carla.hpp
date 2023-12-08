#ifndef CARLA__HPP_
#define CARLA__HPP_

#include <iostream>
#include "mqtt/async_client.h"

enum En_Flag_t
{
    CARLA,
    RPIS,
    ACTIONS,
    SENSORS
};

class Carla : private mqtt::async_client
{
private:
    /* data */
public:
    Carla();
    ~Carla();
    void setConfigFilePath(std::string);
    bool loadConfiguaration();
    void loadDefaultConfiguaration();
    std::string getConfigFilePath();
    mqtt::token_ptr subscribe(void);
    mqtt::delivery_token_ptr publish();
    uint8_t getRxFalg();
    void getData();
    void logData();
};

#endif /* CARLA__HPP_ */