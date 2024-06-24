cmake_minimum_required(VERSION 3.5)
project(sim)

set(CMAKE_CXX_STANDARD 17)


# Find the Paho MQTT C++ package
find_package(PahoMqttCpp REQUIRED)

# Add your source files
set(SOURCE_FILES
  main.cpp
  sim.cpp
  filehandling.cpp 
    )

# Create an executable
add_executable(sim ${SOURCE_FILES})

# Link against the Paho MQTT C++ library
target_link_libraries(sim PahoMqttCpp::paho-mqttpp3)
