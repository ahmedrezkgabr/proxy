#include <iostream>
#include "rpi.hpp"

const std::string DEFAULT_CONFIG_FILEPATH{"./.config"};

/**
 * @brief The main function implementing the core logic of the program.
 *
 * This function initializes an instance of the Rpi class, loads configuration settings,
 * connects to an MQTT broker, subscribes to topics, continuously retrieves and publishes
 * data, and handles exceptions.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 *
 * @return Returns 0 upon successful execution, 1 if an exception occurs.
 */
int main(int argc, char *argv[])
{
    Rpi myRpi;

    /* getting the file of configuaration */
    myRpi.setConfigFilePath((argc > 1) ? std::string(argv[1]) : DEFAULT_CONFIG_FILEPATH);

    /* setting the configuarations */
    std::cout << "Loading configuaration from: " << myRpi.getConfigFilePath() << std::endl;

    if (!myRpi.loadConfiguaration())
    {
        std::cerr << "Unable to load configuaration!" << std::endl;
        std::cerr << "LOADING THE DEFAULT CONFIGUARATION!!" << std::endl;
        myRpi.loadDefaultConfiguaration();
    }

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << myRpi.get_server_uri() << "'..." << std::flush;
        myRpi.connect()->wait();
        std::cout << "OK" << std::endl;

        myRpi.subscribe(); /* will be overloaded to match void input */

        while (true)
        {
            /* get actions' data */
            myRpi.getData();

            /* publish actions' data */
            myRpi.publish();

            if (myRpi.getRxFalg() == SENSORS) /* received from carla */
            {
                /* parsing information */

                /* log the received sensors' data */
                myRpi.logData();

                /* clear the flag */
            }
        }

        /* disconnect and wait till done */
        std::cout << "\nDisconnecting..." << std::flush;
        myRpi.disconnect()->wait();
        std::cout << "OK" << std::endl;
    }
    catch (const std::exception &e)
    {
        /* print the exception error */
        std::cerr << e.what() << '\n';

        return 1;
    }
}