#include <gtest/gtest.h>
#include "../../config.hpp"
class CONFIG_TEST:public testing::Test
{
    public:
    ConfigHandler handler;
    void SetUp()override;
    void TearDown()override;
};