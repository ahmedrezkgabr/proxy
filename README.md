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

### 1. `Proxy/`

- This is the main part of the repo.
- This code is containing the files of call-back functioons and the proxy logic code.
- This code is to be ```refactored``` based on the redesign process.

### 2. `CarlaLikeApplication/`

- This code establishes a way of simulating Carla environment.
- Carla Provides the system with the real-sensors' readings and accepts the actions of each target.
- This code prove this concept using file handling process.

### 3. `TargetLikeApplication/`

- This code establishes a way of simulating Target environment.
- The target accepts the readings of the sensors and provide the actions to be sent back to Carla.
- This code prove this concept using file handling process.

## Installation

You need to clone the rebo, build the code with g++ and link with paho libraries and then run ```mosquitto``` broker and then run the ```proxy```, ```carla```, and some instances of ```target```.

### first you need to clone the repo

```bash
git clone https://github.com/ahmedrezkgabr/proxy.git
cd proxy
```

### build the source code (link with paho cpp)

```bash
g++ -std=c++17 -o Dir_path/Proxy/proxy Dir_path/Proxy/proxy.cpp Dir_path/Proxy/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a

g++ -std=c++17 -o Dir_path/TargetLikeApplication/target Dir_path/TargetLikeApplication/target.cpp Dir_path/TargetLikeApplication/FileHandling.cpp Dir_path/TargetLikeApplication/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a

g++ -std=c++17 -o Dir_path/CarlaLikeApplication/carla Dir_path/CarlaLikeApplication/carla.cpp Dir_path/CarlaLikeApplication/FileHandling.cpp Dir_path/CarlaLikeApplication/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a
```

You now have a three executables ```Dir_path/CarlaLikeApplication/carla```, ```Dir_path/TargetLikeApplication/target```, and ```Dir_path/Proxy/proxy```

### run the broker

```bash
mosquitto -v
```

### run carla

```bash
Dir_path/CarlaLikeApplication/carla
```

### run instances of target

give the instance id, files of logging

```bash
Dir_path/TargetLikeApplication/target 01 file1.csv file2.csv
```

```repeat the last command if you need other instance, change the id and the files```

### run the proxy

```bash
Dir_path/Proxy/proxy
```
