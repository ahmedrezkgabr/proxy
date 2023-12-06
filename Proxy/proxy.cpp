#include <iostream>
#include "mqtt/async_client.h"
#include "MyCallBack.hpp"

const std::string DEFAULT_ADDRESS{"mqtt://localhost:1883"};
const std::string CLIENT_ID{"proxy"};

/*Carla Topics*/
const std::string TOPIC_CARLA_ACTIONS_PUB{"carla/actions"};
const std::string TOPIC_CARLA_SENSORS_SUB{"carla/sensors"};

/*Rpi01 Topics*/
const std::string TOPIC_RPI01_SENSORS_PUB{"rpi/01/sensors"};
const std::string TOPIC_RPI01_ACIONS_SUB{"rpi/01/actions"};

/*Rpi02 Topics*/
const std::string TOPIC_RPI02_SENSORS_PUB{"rpi/02/sensors"};
const std::string TOPIC_RPI02_ACIONS_SUB{"rpi/02/actions"};

/*quality of service*/
const int QOS = 1;
/*creating object duration of 5 seconds to be used for sleeping*/
const auto PERIOD = std::chrono::seconds(5);
const int MAX_BUFFERED_MSGS = 120; // 120 * 5sec => 10min off-line buffering

int main(int argc, char *argv[])
{
    /* get the passed argument to be the address, if not use the defualt */
    std::string address =DEFAULT_ADDRESS;

    /* create a client object */
    mqtt::async_client client(address, CLIENT_ID, MAX_BUFFERED_MSGS, NULL);

    /* create a callBack object and set the callBack */
    MyCallBack callback;
    callback.msg_actions.resize(2);
    client.set_callback(callback);

    /* set connection options */
    auto connOpts = mqtt::connect_options_builder()
                        .keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD)
                        .clean_session(true)
                        .automatic_reconnect(true)
                        .finalize();

    /* creating topics for subscribing and publishing */
                                                            /*carla*/                                       /*Rpi01                                               Rpi02*/
     std::vector<mqtt::topic>top_pub{{client, TOPIC_CARLA_ACTIONS_PUB, QOS, true},{client, TOPIC_RPI01_SENSORS_PUB, QOS, true},{client, TOPIC_RPI02_SENSORS_PUB, QOS, true}};
     std::vector<mqtt::topic>top_sub{{client, TOPIC_CARLA_SENSORS_SUB, QOS, true},{client, TOPIC_RPI01_ACIONS_SUB, QOS, true},{client, TOPIC_RPI02_ACIONS_SUB, QOS, true}};

    /* high level data dealing */
    std::string Carla_Msg;

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << address << "'..." << std::flush;
        client.connect(connOpts)->wait();
        std::cout << "OK" << std::endl;

        /*subscribe TOPIC_CARLA_SENSORS_SUB */
        top_sub[0].subscribe();
        /*subscribe TOPIC_RPI01_ACIONS_SUB */
        top_sub[1].subscribe();
         /*subscribe TOPIC_RPI02_ACIONS_SUB */
        top_sub[2].subscribe();
        std::cout<<"Subsribtion done ..."<<std::endl;
        while (true)
        {
            if (callback.recived_msg_flag & 1) /* received sensors message */
            {
        

                /*publish data to rpi*/
                top_pub[1].publish(callback.msg_sensors);
                top_pub[2].publish(callback.msg_sensors);
                callback.recived_msg_flag &= ~(1);
            }

            if (callback.recived_msg_flag & 2 && callback.recived_msg_flag & 4) /* received actions message */
            {
                
                /*Rpi01+Rpi02*/
                Carla_Msg=callback.msg_actions[0]+","+callback.msg_actions[1];
                top_pub[0].publish(Carla_Msg);
                callback.recived_msg_flag &= ~(6);
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