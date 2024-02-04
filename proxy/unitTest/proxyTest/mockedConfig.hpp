#ifndef MOCKED_CONFIG__H_
#define MOCKED_CONFIG__H_

#include <iostream>
#include <vector>
enum class Config_Error_t
{
    OK,
    NOT_OK
};

using topicsNames_t = std::vector<std::string>;
class ConfigHandler
{
public:
    virtual void setConfigFilePath(std::string) = 0;

    virtual std::string getConfigFilePath() = 0;
    virtual Config_Error_t loadConfiguaration(void) = 0;
    virtual std::string getAddress() = 0;
    virtual std::string getClientID() = 0;
    virtual uint32_t getMaxBufMsgs() = 0;
    virtual bool getCleanSession() = 0;
    virtual bool getAutoReconnect() = 0;
    virtual uint64_t getKeepAliveTime() = 0;
    virtual topicsNames_t getPubTocpicsNames() = 0;
    virtual topicsNames_t getSubTocpicsNames() = 0;
    virtual uint8_t getNumberOfRpis() = 0;
    virtual uint8_t getQualityOfService() = 0;
    virtual bool getRetainedFlag() = 0;
};
#endif /* MOCKED_CONFIG__H_ */