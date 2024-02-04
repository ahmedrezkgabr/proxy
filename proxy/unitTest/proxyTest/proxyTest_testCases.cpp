#include <gmock/gmock-actions.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>
#include "proxyTest_testCases.hpp"
#include "proxy.hpp"
void ProxyTest::SetUp()
{
    EXPECT_CALL(mch, getAddress).Times(1).WillOnce(testing::Return("mqtt://localhost:1883"));
    EXPECT_CALL(mch, getClientID).Times(1).WillOnce(testing::Return("proxy"));
    EXPECT_CALL(mch, getMaxBufMsgs).Times(1).WillOnce(testing::Return(120));
    EXPECT_CALL(mch, getSubTocpicsNames).Times(1).WillOnce(testing::Return(std::vector<std::string>{"carla/sensors", "rpi/01/actions", "rpi/02/actions"}));
    EXPECT_CALL(mch, getPubTocpicsNames).Times(1).WillOnce(testing::Return(std::vector<std::string>{"carla/actions", "rpi/01/sensors", "rpi/02/sensors"}));
    EXPECT_CALL(mch, getQualityOfService).Times(1).WillRepeatedly(testing::Return(1));
    EXPECT_CALL(mch, getRetainedFlag).Times(1).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(mch, getKeepAliveTime).Times(1).WillOnce(testing::Return(600));
    EXPECT_CALL(mch, getCleanSession).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(mch, getAutoReconnect).Times(1).WillOnce(testing::Return(true));
    EXPECT_CALL(mch, getNumberOfRpis).Times(1).WillOnce(testing::Return(2));
    myProxy = new Proxy(mch);
}
void ProxyTest::TearDown()
{
    delete myProxy;
}

TEST_F(ProxyTest, Proxy_Flag)
{

    EXPECT_EQ(myProxy->getRxFalg(), Proxy_Flag_t::NOT);
}

TEST_F(ProxyTest, get_server_uri)
{

    EXPECT_STREQ(myProxy->get_server_uri().c_str(), "mqtt://localhost:1883");
}