#include <iostream>
#include "mqtt/async_client.h"
#include "MyCallBack.hpp"

const std::string DEFAULT_ADDRESS{"mqtt://localhost:1883"};
const std::string CLIENT_ID{"proxy"};

/* Carla's Topics */
const std::string TOPIC_CARLA_ACTIONS_PUB{"carla/actions"};
const std::string TOPIC_CARLA_SENSORS_SUB{"carla/sensors"};

/* Rpi01's Topics */
const std::string TOPIC_RPI01_SENSORS_PUB{"rpi/01/sensors"};
const std::string TOPIC_RPI01_ACTIONS_SUB{"rpi/01/actions"};

/* Rpi02's Topics */
const std::string TOPIC_RPI02_SENSORS_PUB{"rpi/02/sensors"};
const std::string TOPIC_RPI02_ACTIONS_SUB{"rpi/02/actions"};

/* quality of service */
const int QOS = 1;

/* creating object duration of 5 seconds to be used for sleeping */
const auto PERIOD = std::chrono::seconds(5);
const int MAX_BUFFERED_MSGS = 120; // 120 * 5sec => 10min off-line buffering

int main(int argc, char *argv[])
{
    /* init */
    /* get the passed argument to be the address, if not use the defualt */
    std::string address = (argc > 1) ? std::string(argv[1]) : DEFAULT_ADDRESS;

    /* create a client object */ /* NULL for the persistence */
    mqtt::async_client client(address, CLIENT_ID, MAX_BUFFERED_MSGS, NULL);

    /* create a callBack object and set the callBack */
    MyCallBack callback;
    callback.msg.resize(3);
    client.set_callback(callback);

    /* set connection options */
    auto connOpts = mqtt::connect_options_builder()
                        .keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD)
                        .clean_session(true)
                        .automatic_reconnect(true)
                        .finalize();

    /* topics vectors (@ index 0 -> carla), (@ other indeces -> rpi) */
    /* creating topics for publishing */
    std::vector<mqtt::topic> top_pub{{client, TOPIC_CARLA_ACTIONS_PUB, QOS, true},
                                     {client, TOPIC_RPI01_SENSORS_PUB, QOS, true},
                                     {client, TOPIC_RPI02_SENSORS_PUB, QOS, true}};

    /* creating topics for subscribtion */
    std::vector<mqtt::topic> top_sub{{client, TOPIC_CARLA_SENSORS_SUB, QOS, true},
                                     {client, TOPIC_RPI01_ACTIONS_SUB, QOS, true},
                                     {client, TOPIC_RPI02_ACTIONS_SUB, QOS, true}};

    /* high level data dealing */
    std::string msg_senToCarla;

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << address << "'..." << std::flush;
        client.connect(connOpts)->wait();
        std::cout << "OK" << std::endl;

        /* subscribe on the subscribtion topics */
        top_sub[0].subscribe();
        top_sub[1].subscribe();
        top_sub[2].subscribe();

        while (true)
        {
            if (callback.recived_msg_flag & 1) /* received sensors message */
            {
                /* some mechanism to parse the message */

                /* publish data to rpi each with its parsed message */
                top_pub[1].publish(callback.msg[0]);
                top_pub[2].publish(callback.msg[0]);

                /* clear the flag's bit */
                callback.recived_msg_flag &= ~(1);
            }

            /* 6 and 7 indicates 0bxxxxx11x */ /* the size of vector reflects these two numbers */
            /* (2^sizeof(vector)) - 1, and (2^sizeof(vector)) - 2 */
            if ((callback.recived_msg_flag == 6) || (callback.recived_msg_flag == 7)) /* received actions messages of actions */
            {

                /* some mechanism to compose the messages into one message */
                msg_senToCarla = callback.msg[1] + "," + callback.msg[2];

                /* publish the whole message of actions to carla */
                top_pub[0].publish(msg_senToCarla);

                /* clear the flag's bits */
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