
# Proxy

This is demo of a subsystem (Proxy) using mqtt(paho) , this proxy works as an intermediate layer between two systems .



## Test the Proxy

To test the proxy, we made Two applications one to act as carla simulator and the other to act as Raspberry pi  
### Building Proxy Source Code Files
```bash
  g++ -std=c++17 -o Dir_path/Proxy/proxy Dir_path/Proxy/proxy.cpp Dir_path/Proxy/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a
```
### Building Test Applications
```bash
  g++ -std=c++17 -o Dir_path/TargetLikeApplication/target Dir_path/TargetLikeApplication/target.cpp Dir_path/TargetLikeApplication/FileHandling.cpp Dir_path/TargetLikeApplication/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a
```
```bash
  g++ -std=c++17 -o Dir_path/CarlaLikeApplication/carla Dir_path/CarlaLikeApplication/carla.cpp Dir_path/CarlaLikeApplication/FileHandling.cpp Dir_path/CarlaLikeApplication/MyCallBack.cpp -lpaho-mqttpp3 -lpaho-mqtt3a
```
replace Dir_path with your path.

## Authors

- [@Ahmed Rezk](https://github.com/ahmedrezkgabr)
- [@Ahmed Essam](https://github.com/ahmedelmola224)

