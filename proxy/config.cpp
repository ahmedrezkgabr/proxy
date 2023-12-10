#include <iostream>
#include "config.hpp"

ConfigHandler::ConfigHandler(/* args */) {}

ConfigHandler::~ConfigHandler()
{
    /* destroy the vectors */
    this->pubTopicsNames.~vector();
    this->subTopicsNames.~vector();
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
