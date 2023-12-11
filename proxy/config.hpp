#ifndef CONFIG__HHP_
#define CONFIG__HHP_
#include <iostream>
#include <chrono>
#include "boost/property_tree/ini_parser.hpp"

enum class Config_Error_t
{
    OK,
    NOT_OK
};

/* data */
struct ClientData
{
    std::string address;
    std::string clientId;
    uint32_t maxBufMsgs;
    bool cleanSession;
    bool autoReconnect;
    uint64_t keepAliveTime;
};

class ConfigHandler
{
public:
    ConfigHandler(/* args */);
    ~ConfigHandler();

    /**
     * @brief Sets the configuration File Path.
     *
     * @param filePath as string
     * @return An enum of Proxy_Error_t.
     */
    void setConfigFilePath(std::string);

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
    Config_Error_t loadConfiguaration(void);

    std::string getAddress();
    std::string getClientID();
    uint32_t getMaxBufMsgs();
    bool getCleanSession();
    bool getAutoReconnect();
    uint64_t getKeepAliveTime();

    std::vector<std::string> getPubTocpicsNames();
    std::vector<std::string> getSubTocpicsNames();

    uint8_t getNumberOfRpis();
    uint8_t getQualityOfService();
    bool getRetainedFlag();

    std::string getConfigFilePath();

private:
    /* client */
    ClientData myClientData;

    /* topics */
    std::vector<std::string> pubTopicsNames;
    std::vector<std::string> subTopicsNames;
    uint8_t numberOfRpis;
    uint8_t qualityOfService;
    bool retainedFlag;

    std::string configFilePath{"./../config.ini"};
};

#endif /* CONFIG__HHP_ */