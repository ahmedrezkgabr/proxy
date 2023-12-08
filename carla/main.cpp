#include <iostream>
#include "carla.hpp"

const std::string DEFAULT_CONFIG_FILEPATH{"./.config"};

/**
 * @brief The main function implementing the core logic of the program.
 *
 * This function initializes a Carla object, loads configuration settings,
 * connects to an MQTT broker, subscribes to topics, retrieves sensor data,
 * publishes sensor data, and handles received actions. It runs indefinitely
 * until manually terminated.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 *
 * @return Returns 0 on successful execution, and 1 if an exception occurs.
 *
 * @note The first command-line argument (if provided) is assumed to be the
 *       configuration file path; otherwise, a default path is used.
 *
 * @see Carla
 */
int main(int argc, char *argv[])
{
    Carla myCarla;

    /* getting the file of configuaration */
    myCarla.setConfigFilePath((argc > 1) ? std::string(argv[1]) : DEFAULT_CONFIG_FILEPATH);

    /* setting the configuarations */
    std::cout << "Loading configuaration from: " << myCarla.getConfigFilePath() << std::endl;

    if (!myCarla.loadConfiguaration())
    {
        std::cerr << "Unable to load configuaration!" << std::endl;
        std::cerr << "LOADING THE DEFAULT CONFIGUARATION!!" << std::endl;
        myCarla.loadDefaultConfiguaration();
    }

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << myCarla.get_server_uri() << "'..." << std::flush;
        myCarla.connect()->wait();
        std::cout << "OK" << std::endl;

        myCarla.subscribe(); /* will be overloaded to match void input */

        while (true)
        {
            /* get sensors' data */
            myCarla.getData();

            /* publish sensors' data */
            myCarla.publish();

            if (myCarla.getRxFalg() == ACTIONS) /* received from rpi */
            {
                /* parsing information */

                /* logging received actions' data */
                myCarla.logData();

                /* clear the flag */
            }
        }

        /* disconnect and wait till done */
        std::cout << "\nDisconnecting..." << std::flush;
        myCarla.disconnect()->wait();
        std::cout << "OK" << std::endl;
    }
    catch (const std::exception &e)
    {
        /* print the exception error */
        std::cerr << e.what() << '\n';

        return 1;
    }
}