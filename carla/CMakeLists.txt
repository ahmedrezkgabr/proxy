cmake_minimum_required(VERSION 3.5)
project(carla)

set(CMAKE_CXX_STANDARD 17)

# Find the Paho MQTT C++ package
find_package(PahoMqttCpp REQUIRED)

# Add your source files
set(SOURCE_FILES
  main.cpp
  carla.cpp
  filehandling.cpp 
    )

# Create an executable
add_executable(carla ${SOURCE_FILES})

# Link against the Paho MQTT C++ library
target_link_libraries(carla PahoMqttCpp::paho-mqttpp3)
