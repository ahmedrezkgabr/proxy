cmake_minimum_required(VERSION 3.5)
project(proxy)

set(CMAKE_CXX_STANDARD 17)

# Find the Paho MQTT C++ package
find_package(PahoMqttCpp REQUIRED)

# Add your source files
set(SOURCE_FILES
  config.cpp
  main.cpp
  proxy.cpp 
    )

# Create an executable
add_executable(proxy ${SOURCE_FILES})

# Link against the Paho MQTT C++ library
target_link_libraries(proxy PahoMqttCpp::paho-mqttpp3)

