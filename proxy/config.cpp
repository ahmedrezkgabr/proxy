#include <iostream>
#include "config.hpp"

ConfigHandler::ConfigHandler(/* args */) {}

ConfigHandler::~ConfigHandler()
{
    /* destroy the vectors */
    this->myTopicsData.pubTopicsNames.~vector();
    this->myTopicsData.subTopicsNames.~vector();
}

/**
 * @brief Sets the configuration File Path.
 *
 * @param filePath as string
 * @return An enum of Proxy_Error_t.
 */
void ConfigHandler::setConfigFilePath(std::string filePath)
{
    /* set the configuration file path to be the passed parameter */
    this->configFilePath = filePath;
}

/**
 * @brief Gets the configuration File Path.
 *
 * @return std::string as the file path.
 */
std::string ConfigHandler::getConfigFilePath(void)
{
    /* return the configuration file path */
    return this->configFilePath;
}

/**
 * @brief Loads the Configuaration.
 *
 * Loads the Configuaration parameters from the configuation file
 * These configuartion ()
 *
 * @return An enum of Proxy_Error_t.
 */
Config_Error_t ConfigHandler::loadConfiguaration(void)
{
    /* create the configuration parameters tree */
    boost::property_tree::ptree configTree;

    /* fill the tree with the content at the configuration file */
    boost::property_tree::ini_parser::read_ini(this->configFilePath, configTree);

    /* get the client parameters */
    this->myClientData.address = configTree.get<std::string>("client.address");
    this->myClientData.clientId = configTree.get<std::string>("client.clientId");
    this->myClientData.maxBufMsgs = configTree.get<uint32_t>("client.maxBufMsgs");
    this->myClientData.cleanSession = configTree.get<bool>("client.cleanSession");
    this->myClientData.autoReconnect = configTree.get<bool>("client.autoReconnect");
    this->myClientData.keepAliveTime = configTree.get<uint64_t>("client.keepAliveTime");
    
    this->myTopicsData.qualityOfService = configTree.get<uint8_t>("topics.qualityOfService");
    this->myTopicsData.retainedFlag = configTree.get<bool>("topics.retainedFlag");

    /* get the number of the rpis in the system */
    this->myTopicsData.numberOfRpis = configTree.get<uint8_t>("topics.numberOfRpis");

    /* get carla's topics */
    this->myTopicsData.pubTopicsNames.push_back(configTree.get<std::string>("topics.carlaActionsTopic"));
    this->myTopicsData.subTopicsNames.push_back(configTree.get<std::string>("topics.carlaSensorsTopic"));

    /* loop to get rpix topics : x = [1:numberOfRpis] */
    for (uint8_t i = 0; i < this->myTopicsData.numberOfRpis; i++)
    {
        this->myTopicsData.pubTopicsNames.push_back(configTree.get<std::string>("topics.rpi0" + std::to_string(i + 1) + "SensorsTopic"));
        this->myTopicsData.subTopicsNames.push_back(configTree.get<std::string>("topics.rpi0" + std::to_string(i + 1) + "ActionsTopic"));
    }

    return Config_Error_t::OK;
}

std::string ConfigHandler::getAddress()
{
    return myClientData.address;
}