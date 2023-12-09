#include <iostream>
#include "boost/property_tree/ini_parser.hpp"
#include "mqtt/async_client.h"
#include "proxy.hpp"

/**
 * @brief Sets the configuration File Path.
 *
 * @param filePath as string
 * @return An enum of Proxy_Error_t.
 */
Proxy_Error_t Proxy::setConfigFilePath(std::string filePath)
{
    this->configFilePath = filePath;
    return Proxy_Error_t::OK;
}