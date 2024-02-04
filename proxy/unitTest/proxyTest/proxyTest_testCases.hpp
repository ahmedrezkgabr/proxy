#ifndef TEST__HHP_
#define TEST__HHP_
#include "mockedConfig.hpp"
#include <gmock/gmock.h>
#include "proxy.hpp"


class MockConfigHandler : public ConfigHandler
{
public:
    // Mocking the pure virtual methods from ConfigHandler
    MOCK_METHOD(void, setConfigFilePath, (std::string), (override));
    MOCK_METHOD(std::string, getConfigFilePath, (), (override));
    MOCK_METHOD(Config_Error_t, loadConfiguaration, (), (override));

    MOCK_METHOD(std::string, getAddress, (), (override));
    MOCK_METHOD(std::string, getClientID, (), (override));
    MOCK_METHOD(uint32_t, getMaxBufMsgs, (), (override));
    MOCK_METHOD(bool, getCleanSession, (), (override));
    MOCK_METHOD(bool, getAutoReconnect, (), (override));
    MOCK_METHOD(uint64_t, getKeepAliveTime, (), (override));

    MOCK_METHOD(topicsNames_t, getPubTocpicsNames, (), (override));
    MOCK_METHOD(topicsNames_t, getSubTocpicsNames, (), (override));

    MOCK_METHOD(uint8_t, getNumberOfRpis, (), (override));
    MOCK_METHOD(uint8_t, getQualityOfService, (), (override));
    MOCK_METHOD(bool, getRetainedFlag, (), (override));
};

class ProxyTest : public testing::Test
{
public:
    Proxy *myProxy;
    MockConfigHandler mch;
    void SetUp() override;
    void TearDown() override;
};
#endif  /* CONFIG__HHP_ */