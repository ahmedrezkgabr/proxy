# Proxy

## Abstract

Welcome to the Proxy Subsystem of the Truck Platooning Graduation Project! This subsystem serves as a crucial intermediate layer, facilitating seamless communication between the simulation world and the real world components of the Truck Platooning system.

## Key Features

### 1. `Communication Middleware`

The Proxy Subsystem incorporates a robust communication middleware to efficiently transfer messages between the simulation world and real-world components.

### 2. `Message Translation`

It is responsible for translating messages between the different formats used in the simulation and physical systems, ensuring seamless interoperability.

### 3. `Real-time Interaction`

Enables real-time interaction between the simulated and physical components, allowing for dynamic adjustments and responsiveness within the Truck Platooning system.

### 4. `MQTT broker`

This proxy communicate over ```MQTT``` protocol, and uses ```Paho-mqtt``` library to establish a client to the proxy.

## Directories

### 1. `proxy/`

- This is the main part of the repo.
- This code is containing the files of `CMakeLists.txt`, `config.cpp`, `config.hpp`, `config.ini`, `proxy.cpp`, `proxy.hpp`, and `main.cpp`.
- `CMakeLists.txt` to build the code.
- `config.cpp`, `config.hpp`, `config.ini` to configure the parameters.
- in `config.ini` you can specify the number of targets, AKA `raspberrypi`, and other things related to the network.
- `proxy.cpp`, `proxy.hpp` provide the heart of the system and how it deals with `MQTT` network.
- `main.cpp` provides the main logic of the system.

### 2. `carla/`

- This code establishes a way of making an application that acts like `carla simulator` environment.
- `carla simulator` Provides the system with the real-sensors' readings and accepts the actions of each target.
- This code prove this concept using file handling process.

### 3. `rpi/`

- This code establishes a way of making an application that acts like `target AKA raspberrypi`.
- The target accepts the readings of the sensors and provide the actions to be sent back to Carla.
- This code prove this concept using file handling process.

## Installation

### not yet
