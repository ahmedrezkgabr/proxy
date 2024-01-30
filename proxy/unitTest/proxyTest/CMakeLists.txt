cmake_minimum_required(VERSION 3.5)
project(proxy)

set(CMAKE_CXX_STANDARD 17)

# Find the Paho MQTT C++ package
find_package(GTest REQUIRED)
find_package(PahoMqttCpp REQUIRED)

# Add your source files
set(SOURCE_FILES
  proxyTest_main.cpp
  proxyTest_testCases.cpp
  ./../../proxy.cpp
  ./../../config.cpp
    )

# Create an executable
add_executable(proxyTest ${SOURCE_FILES})

# Link against GTest
target_link_libraries(proxyTest GTest::GTest GTest::Main)
target_link_libraries(proxyTest PahoMqttCpp::paho-mqttpp3)

