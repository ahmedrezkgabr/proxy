# proxy
this is the demo of a subsystem **(Proxy)**
this proxy works as an intermediate layer between the real world processing systems and the simulation tool

the simulation tool is Carla simulator

this repo consists of 3 directories 
## 1. Proxy
    this code is made to test the other test applications (perform the role of broxy **route the messages between carla and the real world**)
## 2. CarlaLikeApplication
    this application make the role of carla in the point of view of proxy **send the sensors to proxy and receives the actions from proxy**
## 3. TargetLikeApplication
    this code make the role of real target application in the point of view of proxy **receives routed sensors data, process it, send the corresponding actions to the proxy**

### how to test
* first you need to clone the repo
``` bash
git clone https://github.com/proxy
cd proxy
```
- then you need to build each application using g++ and link with paho libraries

``` bash
g++ -std=c++17 -o ./CarlaLikeApplication/carla ./CarlaLikeApplication/carla.cpp ./CarlaLikeApplication/FileHandling.cpp ./CarlaLikeApplication/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a

g++ -std=c++17 -o ./TargetLikeApplication/target ./TargetLikeApplication/target.cpp ./TargetLikeApplication/FileHandling.cpp ./TargetLikeApplication/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a

g++ -std=c++17 -o ./Proxy/proxy ./Proxy/proxy.cpp ./Proxy/FileHandling.cpp ./Proxy/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a
```
- then run each application in a seperate terminal and do not forget to run the MQTT broker also