#include <gtest/gtest.h>
#include "../../config.hpp"

TEST(CONFIG_TEST, getConfigFilePath_1)
{
    // arrange
    ConfigHandler handler;
    std::string path{"../../TestPath"};
    // actions
    handler.setConfigFilePath(path);
    // expect
    EXPECT_STREQ(path.c_str(), handler.getConfigFilePath().c_str());
}

TEST(CONFIG_TEST, getConfigFilePath_2)
{ // arrange
    ConfigHandler handler;
    std::string path{"../../TestPath"};
    // actions
    handler.setConfigFilePath(path);
    // expect
    EXPECT_STREQ(path.c_str(), handler.getConfigFilePath().c_str());
}