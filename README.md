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

---
---

## Dependancies

`libssl-dev`, `cmake`, `libboost-all-dev`, `libpaho-mqtt*`, `libgtest-dev`, and `libgmock-dev`.

## usage

### clone the repo

```bash
git clone https://github.com/ahmedrezkgabr/proxy.git

cd proxy
```

### configure the proxy

- these configuration are in `./proxy/config.ini` file.

### build the proxy

```bash
cd proxy
mkdir build
cd build

cmake ..
make
```

- you get an executable file called `proxy`.

### run the proxy

```bash
./proxy
```

- you can pass another configuration file as an argument to the proxy

```bash
./proxy <path to the configuration file>
```

---
---

## Source Code

### `proxy/`

```txt
.
├── CMakeLists.txt
├── config.cpp
├── config.hpp
├── config.ini
├── main.cpp
├── proxy.cpp
├── proxy.hpp
└── unitTest
    ├── configTest
    └── proxyTest

```

- the system consists of two main modules `config` and `proxy`.

#### `config`

- the config module is responsible for dynamic configuration of the system.
- these configurations are:

```txt
these configuration are mqtt-client-configurations
and the topics which the proxy-client is dealing with
```

`address`, `clientId`, `maxBufMsgs`, `cleanSession`, `autoReconnect`, `keepAliveTime`,`qualityOfService`, `retainedFlag`, `numberOfRpis`, `carlaSensorsTopic`, `carlaActionsTopic`,  `rpi01SensorsTopic`, `rpi01ActionsTopic`, ... `rpixxSensorsTopic`, `rpixxActionsTopic`.

- xx in the topic name is depending on the `numberOfRpis` preperity

---

#### `proxy`

this code provide the main functionality of the proxy-mqtt-client that is responsible to establish the network between the `caral` world and the `rpi` worlds.

---

## Testing

### unit testing

- each module has its own testing directory

### integration testing

- carla/ and rpi/ are mock applications for the integration testing

### 2. `carla/`

- This code establishes a way of making an application that acts like `carla simulator` environment.
- `carla simulator` Provides the system with the real-sensors' readings and accepts the actions of each target.
- This code prove this concept using file handling process.

### 3. `rpi/`

- This code establishes a way of making an application that acts like `target AKA raspberrypi`.
- The target accepts the readings of the sensors and provide the actions to be sent back to Carla.
- This code prove this concept using file handling process.

## Installation and testing

### clone the repo

```bash
git clone https://github.com/ahmedrezkgabr/proxy.git

cd proxy
```

### configure the proxy system, and create logging files for test applications

- open ./proxy/config.ini with any text editor

- make your modifications

- save the modified file

- make directory to contain logging files

```bash
mkdir .log
cd .log

# make directory for each test app
# carla
mkdir carla
touch carla/sensors.csv
touch carla/actions.csv

# rpixx
mkdir rpi01
touch rpi01/sensors.csv
touch rpi01/actions.csv

# repeate the last three commands with diferent numbers 02, 03, ...
```

- as mentioned before **carla and each rpi systems are testing the core of this repo ```proxy```**

### build the code

#### install dependencies

- you should install ```cmake```, ```make```, ```gcc```, ```paho-mqtt-cpp```, ```boost/property_tree/ini_parser``` and ```mqtt-broker(mosquitto)```.

#### build proxy

- make a directory to contain the building process output files

```bash
mkdir ./proxy/build
cd ./proxy/build/
```

- generate cmake files

```bash
cmake ..
```

- build the code by make command

```bash
make
```

- now you have executable named ```proxy```

#### build carla

- make a directory to contain the building process output files

```bash
mkdir ./carla/build
cd ./carla/build/
```

- generate cmake files

```bash
cmake ..
```

- build the code by make command

```bash
make
```

- now you have executable named ```carla```

#### build rpi

- make a directory to contain the building process output files

```bash
mkdir ./rpi/build
cd ./rpi/build/
```

- generate cmake files

```bash
cmake ..
```

- build the code by make command

```bash
make
```

- now you have executable named ```rpi```

### run the system

#### run the `mqtt broker`

```bash
# -v for logging information
mosquitto -v
```

#### run the ```proxy```

- navigate to the build directory of proxy

- run the proxy

```bash
# you can have your own config.ini file
# if thouhg, pass its path as an argument to the proxy
./proxy
```

#### run test applications ```carla```, and some instences of ```rpi```

- navigate to the build directory of carla

- run carla

```bash
./carla
```

- navigate to the build directory of rpi

- run some instances of rpi

- give some arguments ```rpi-id```, ```path-of-actions-logging-file```, ```path-of-sensors-logging-file```

- repeate the command with different argument

- rpi-id is a sequence number ```01```, ```02```, ```xx```

```bash
./rpi <rpi-id> <path-of-actions-file> <path-of-sensors-file>
```
