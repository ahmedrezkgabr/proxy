#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "mqtt/async_client.h"
#include "MyCallBack.hpp"

const std::string DFLT_ADDRESS{"mqtt://localhost:1883"};
const std::string CLIENT_ID{"proxy"};

const std::string TOPIC_CARLA_ACTIONS_PUB{"carla/actions"};
const std::string TOPIC_CARLA_SENSORS_SUB{"carla/sensors"};

const std::string TOPIC_RPI_SENSORS_PUB{"rpi/01/sensors"};
const std::string TOPIC_RPI_ACIONS_SUB{"rpi/01/actions"};

const int QOS = 1;

const auto PERIOD = std::chrono::seconds(5);
const int MAX_BUFFERED_MSGS = 120; // 120 * 5sec => 10min off-line buffering

int main(int argc, char *argv[])
{
    /* get the passed argument to be the address, if not use the defualt */
    std::string address = (argc > 1) ? std::string(argv[1]) : DFLT_ADDRESS;

    /* create a client object */
    mqtt::async_client client(address, CLIENT_ID, MAX_BUFFERED_MSGS, NULL);

    /* create a callBack object and set the callBack */
    MyCallBack callback;
    client.set_callback(callback);

    /* set connection options */
    auto connOpts = mqtt::connect_options_builder()
                        .keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD)
                        .clean_session(true)
                        .automatic_reconnect(true)
                        .finalize();

    /* creating topics for subscribing and publishing */
    mqtt::topic top_carla_sensors_sub(client, TOPIC_CARLA_SENSORS_SUB, QOS, true);
    mqtt::topic top_rpi_actions_sub(client, TOPIC_RPI_ACIONS_SUB, QOS, true);
    mqtt::topic top_carla_actions_pub(client, TOPIC_CARLA_ACTIONS_PUB, QOS, true);
    mqtt::topic top_rpi_sensors_pub(client, TOPIC_RPI_SENSORS_PUB, QOS, true);

    /* high level data dealing */
    std::string sensors, actions;

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << address << "'..." << std::flush;
        client.connect(connOpts)->wait();
        std::cout << "OK" << std::endl;

        /* subscribe for the actions messages */
        top_carla_sensors_sub.subscribe();
        top_rpi_actions_sub.subscribe();
        while (true)
        {
            if (callback.recived_msg_flag & 1) /* received sensors message */
            {
                top_rpi_sensors_pub.publish(callback.msg_sensors);
                callback.recived_msg_flag &= ~(1);
            }

            if (callback.recived_msg_flag & 2) /* received actions message */
            {
                top_carla_actions_pub.publish(callback.msg_actions);
                callback.recived_msg_flag &= ~(2);
            }
        }

        /* disconnect and wait till done */
        std::cout << "\nDisconnecting..." << std::flush;
        client.disconnect()->wait();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception &exc)
    {
        /* print the exception error */
        std::cerr << exc.what() << std::endl;
        return 1;
    }

    return 0;
}