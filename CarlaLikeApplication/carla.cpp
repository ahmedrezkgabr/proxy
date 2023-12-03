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
// #include "FileHandling.hpp"
#include "MyCallBack.hpp"

const std::string DFLT_ADDRESS{"mqtt://localhost:1883"};
const std::string CLIENT_ID{"carla"};
const std::string TOPIC_TO_PUB{"carla/sensors"};
const std::string TOPIC_TO_SUB{"carla/actions"};

const int QOS{1};

const std::string INPUT_FILE_PATH{"./sensors.csv"};
const std::string OUTPUT_FILE_PATH{"./actions.csv"};

const auto PERIOD = std::chrono::seconds(5);
const int MAX_BUFFERED_MSGS = 120; // 120 * 5sec => 10min off-line buffering

class FileHandling
{
public:
    std::string getData(const std::string fileName);
    void setData(const std::string data, const std::string fileName);
    FileHandling(/* args */);
    ~FileHandling();
};


FileHandling::FileHandling() {}
FileHandling::~FileHandling() {}

std::string FileHandling::getData(const std::string fileName)
{
    /* open the file as input stream */
    std::ifstream inputFile(fileName);

    /* is it opend well? */
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file." << std::endl;
        return NULL;
    }

    /* read the data */
    std::string data;
    std::getline(inputFile, data);

    /* close the file */
    inputFile.close();
    return data;
}

void FileHandling::setData(const std::string data, const std::string fileName)
{
    /* open the file as output stream */
    std::ofstream outputFile(fileName);

    /* is it opend well? */
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening the file." << std::endl;
    }

    /* write the data */
    outputFile << data;

    /* close the file */
    outputFile.close();
}

int main(int argc, char *argv[])
{
    /* init */
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

    /* creating the topics */
    mqtt::topic topForBub(client, TOPIC_TO_PUB, QOS, true);
    mqtt::topic topForSub(client, TOPIC_TO_SUB, QOS, true);

    /* high level data dealing */
    FileHandling handler;
    std::string sensors;

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << address << "'..." << std::flush;
        client.connect(connOpts)->wait();
        std::cout << "OK" << std::endl;

        /* subscribe for the actions messages */
        topForSub.subscribe();
        while (true)
        {
            /* getting sensors data from carla */
            sensors = handler.getData(INPUT_FILE_PATH);

            /* publish the new data each to its topic */
            topForBub.publish(sensors);

            if (callback.recived_msg_flag)
            {
                handler.setData(callback.msg, OUTPUT_FILE_PATH);
                callback.recived_msg_flag = 0;
            }

            /* wait */
            sleep(2);
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