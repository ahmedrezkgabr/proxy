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

// enum class States_t : uint8_t
// {
//     INIT_STATE,
//     IDLE_STATE,
//     PUBLISH_STATE,
//     SEND_MSG_TO_CARLA_STATE
// };

// int main()
// {

//     States_t state = States_t::INIT_STATE;
//     while (true)
//     {
//         switch (state)
//         {
//         case States_t::INIT_STATE:
//             /* init */
//             /* subscribe */
//             /* move to the next state */
//             break;
//         case States_t::IDLE_STATE:
//             /* get data from carla */
//             /* validate the data */
//             /* check messages arrivals */
//             /* move to the next sate */
//             break;
//         case States_t::PUBLISH_STATE:
//             /* publish */
//             /* check sending */
//             /* move to the next state */
//             break;
//         case States_t::SEND_MSG_TO_CARLA_STATE:
//             /* send data to carla */
//             /* move to the next state */
//             break;
//         default:
//             break;
//         }
//     }
// }

// int main()
// {
//     /* init */
//     /* client initialization */
//     /* connection options, topics, call_backs */

//     /* client connect */

//     /* subscribe on some topics which associated to the client */

//     /* loop */
//     /* get data from carla */
//     /* validate the data */
//     /* publish data to some topics which associated to the client */

//     /* check on message arrival flag (setted @ call back) */
//     /* send messages to carla */
// }
// class MyCallback : public virtual mqtt::callback
// {
// public:                                                              /* overriding the functions on every action may happen */
//     void connection_lost(const std::string &cause) override;         /* call back function called when lossing the connection with the broker */
//     void message_arrived(mqtt::const_message_ptr msg) override;      /* call back function called when receiving a message */
//     void delivery_complete(mqtt::delivery_token_ptr token) override; /* call back function called when delivery complete */
// };

// class BrokerHandler
// {
// private:
//     mqtt::async_client client;
//     mqtt::connect_options conOpts;
//     MyCallback callbacks;
//     /* vectors */
//     mqtt::topic pub_topics[4];
//     mqtt::topic sub_topics[4];
//     mqtt::const_message_ptr msgs[4]; /* filled with the callback */

// public:
//     BrokerHandler(/* args */);
//     BrokerHandler(mqtt::async_client, mqtt::connect_options, mqtt::topic[], mqtt::topic[], MyCallback);
//     ~BrokerHandler();
//     bool connect();
//     void listen();
//     std::string getData();       /* read the subscribed data */
//     void setData(std::string[]); /* publish the passed data */
//     void addTopic(mqtt::topic, mqtt::topic[]);
//     void removeTopic(mqtt::topic, mqtt::topic[]);
// };

// class CarlaHandler
// {
// private:
//     /* vectors */
//     std::string inputFilePath[4];
//     std::string outputFilePath[4];

// public:
//     CarlaHandler(/* args */);
//     ~CarlaHandler();
//     std::string getData();
//     void setData(std::string[]);
// };

// int main()
// {
//     /* create parameters objects */
//     BrokerHandler bh(/* params */);
//     CarlaHandler carla(/* params*/);

//     bh.connect();
//     bh.listen();

//     /* data vectors */

//     while (true)
//     {
//         data = carla.getData();
//         /* validation and operations */
//         bh.setData(data); /* buplish the vector data */

//         if (flag) /* indecates that the vector elements have been filled */
//         {
//             carla.setDta(bh.getData());
//         }
//     }
// }