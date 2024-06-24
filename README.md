# Proxy Software Component

## Overview

This repository contains the source code for the "proxy" software component, a crucial part of the graduation project "Vehicle Platooning with V2V using Embedded Linux". The main objective of this project is to implement a truck platooning system, which involves realistic simulations and embedded Linux target units to represent the brains of vehicles responsible for communication and control algorithms.

## Project Components

The system consists of:

- **Simulation Environment (CARLA)**: Provides a realistic environment for testing the system.
- **Embedded Linux Target Units**: Each unit represents a vehicle's brain, handling communication and control algorithms.

## Problem and Solution

The project faced a challenge with unsynchronized data exchange between the simulation environment and the target units. The proxy component was introduced to address this issue by synchronizing data transmission and reception between the simulation environment (CARLA) and the embedded Linux target units.

## Proxy Component

The proxy component's primary role is to:

1. **Receive Sensor Messages**: Collect sensor data from the CARLA simulation environment.
2. **Parse and Route Messages**: Parse the sensor data and route the parsed messages to the corresponding target embedded Linux units.
3. **Synchronize Actions**: Receive control signals from the target units, compose them into a single message, and send it back to CARLA to synchronize the arrival of actions.

## Communication Protocol

The communication between the simulation environment, proxy, and target units is based on the MQTT protocol using Mosquitto broker and Paho clients.

## Repository Structure

``` bash
├── proxy
│   └── unitTest
│       ├── configTest
│       │   └── coverageReports
│       └── proxyTest
│           └── coverageReports
├── sim
├── trgt
├── .git
└── .github
    └── workflows
```

``` bash
proxy
├── CMakeLists.txt
├── config.cpp
├── config.hpp
├── config.ini
├── main.cpp
├── proxy.cpp
├── proxy.hpp
└── unitTest
    ├── configTest
    │   ├── ...
    │   ├── ...
    │   └── coverageReports
    │       ├── ...
    │       └── ...
    └── proxyTest
        ├── ...
        ├── ...
        └── coverageReports
            ├── ...
            └── ...        
```

``` bash
trgt
├── CMakeLists.txt
├── filehandling.cpp
├── filehandling.hpp
├── main.cpp
├── trgt.cpp
└── trgt.hpp

sim
├── CMakeLists.txt
├── filehandling.cpp
├── filehandling.hpp
├── main.cpp
├── sim.cpp
└── sim.hpp
```

### Directories Description

- **proxy**: Contains the source code for the proxy component.
- **sim**: Standalone application acting as the simulation environment for the proxy for the integration testing usage.
- **trgt**: Standalone application acting as the target for the proxy for the integration testing usage.
- **unitTest**: Contains unit test cases using GoogleTest.
- **.github**: Contains CI/CD pipeline configurations using GitHub Actions.

## Building and Running

### Dependencies

The Proxy software has the following dependencies:
`cmake`, `paho client lib`, `boost lib` and for unit testing: `gtest lib`, `gmock lib`

you can install them using

``` bash
sudo apt-get update && sudo apt-get install -y libssl-dev cmake libboost-all-dev libpaho-mqtt* libgtest-dev libgmock-dev
```

To build and run the proxy component, follow these steps:

1. **Clone the repository**:

   ```bash
   git clone https://github.com/ahmedrezkgabr/proxy.git
   cd proxy
   ```

2. **Build the proxy component**:

   ```bash
   cd proxy
   # optional: confiuarate the proxy behaviour from config.ini file
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the proxy component**:

    The running of proxy needs the MQTT brocker to be active

   ```bash
   # you can pass your ini configuration file
   ./proxy <path/to/configuration/file>
   ```

## Unit Testing

Unit tests are provided using the GoogleTest framework. To build and run the tests:

1. **Navigate to the unit test directory**:

   ```bash
   cd proxy/unitTest/configTest
   ```

2. **Build the tests**:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the tests**:

   ```bash
   ./configTest
   ```

4. **Repeat for the proxyTest**

## Integration Testing

We have a stand-alone applications that provide the functionality of the target and simulation from the proxy POV (communication).

To test the Interaction between proxy, simulation, and number of targets

1. **Navigate to sim and build the simulation-like application**:

    ``` bash
    cd sim
    mkdir build
    cd build
    cmake ..
    make
    ```

2. **Run the simulation-like application**

    The running of simulation-like application needs the MQTT brocker to be active

   ```bash
   ./sim <address>
   ```

3. **Navigate to trgt and build the target-like application**:

    ``` bash
    cd trgt
    mkdir build
    cd build
    cmake ..
    make
    ```

4. **Run the target-like application**:

    The running of target-like application needs the MQTT brocker to be active

   ```bash
   ./trgt <target_id> <path/to/actions/log/file> <path/to/sensors/log/file>
   ```

5. **You can add instences of the target like application as you want, each with unique id**:
    
    you need to specify the number of targets in the proxy configuration file `proxy/proxy/confi.ini`.


6. **Do not forget to build the log directory for the sim and trgt applications and touch files for both sensors and actions**:

    bath prefered to be like:
    ```bash
    ├── proxy
    ├── sim
    ├── trgt
    └── .log
    ```

## CI/CD

The repository uses GitHub Actions for continuous integration and continuous deployment (CI/CD). The configuration files are located in the `.github/workflows` directory.
