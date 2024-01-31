#include "configTest_testCases.hpp"

void configTest::SetUp() {}
void configTest::TearDown() {}

TEST_F(configTest, getConfigFilePath)
{
    /* arrange */
    std::string path{"../../TestPath"};

    /* act */
    handler.setConfigFilePath(path);

    /* expect */
    EXPECT_STREQ(path.c_str(), handler.getConfigFilePath().c_str());
}

TEST_F(configTest, loadConfiguaration)
{
    /* arrange */
    std::string path{"./../../../config.ini"};

    /* act */
    handler.setConfigFilePath(path);

    /* expect */
    EXPECT_EQ(Config_Error_t::OK, handler.loadConfiguaration());
}

TEST_F(configTest, getAddress)
{
    /* arrange */
    std::string address{"mqtt://localhost:1883"};
    std::string path{"./../../../config.ini"};

    /* act */
    handler.setConfigFilePath(path);
    handler.loadConfiguaration();

    /* expect */
    EXPECT_STREQ(address.c_str(), handler.getAddress().c_str());
}