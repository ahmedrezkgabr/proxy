cmake_minimum_required(VERSION 3.5)
project(proxy)

set(CMAKE_CXX_STANDARD 17)

# Find the Paho MQTT C++ package
find_package(GTest REQUIRED)
find_package(Boost REQUIRED)
add_definitions(-Dgtest_disable_pthreads=OFF)
set(gtest_disable_pthreads ON)

# Add your source files
set(SOURCE_FILES
  configTest_main.cpp
  configTest_testCases.cpp
  ./../../config.cpp
    )

# Create an executable
add_executable(configTest ${SOURCE_FILES})

# Link against GTest
target_link_libraries(configTest GTest::GTest GTest::Main)

