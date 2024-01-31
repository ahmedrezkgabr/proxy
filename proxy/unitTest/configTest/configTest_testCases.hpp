#include <gtest/gtest.h>
#include "../../config.hpp"
class configTest:public testing::Test
{
    public:
    std::mutex myMutex;
    ConfigHandler handler;
    void SetUp()override;
    void TearDown()override;
};