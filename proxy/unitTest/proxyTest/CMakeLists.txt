cmake_minimum_required(VERSION 3.5)
project(proxy)

set(CMAKE_CXX_STANDARD 17)

# Find Gtest, Gmock, pahoMqtt libraries
find_package(GTest REQUIRED)
find_package(libgmock-dev REQUIRED)
find_package(PahoMqttCpp REQUIRED)
# add_library(libgmock-dev)

# Add your source files
set(SOURCE_FILES
  proxyTest_main.cpp
  proxyTest_testCases.cpp
  proxy.cpp
)

# Create an executable
add_executable(proxyTest ${SOURCE_FILES})

# Link against GTest
target_link_libraries(proxyTest PUBLIC GTest::GTest GTest::Main PahoMqttCpp::paho-mqttpp3)