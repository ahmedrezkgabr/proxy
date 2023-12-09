#include <iostream>
#include "proxy.hpp"

const std::string DEFAULT_CONFIG_FILEPATH{"./.config"};

/**
 * @brief The main function that implements the core logic of the program.
 *
 * This function initializes a Proxy object, loads configuration from the command line or uses a default filepath,
 * connects to an MQTT broker, subscribes to topics, and handles incoming messages to perform communication between
 * components (CARLA and RPIS). It also catches exceptions and prints error messages if any occur during execution.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 *
 * @return Returns 0 on successful execution, or 1 if an exception occurs.
 */
int main(int argc, char *argv[])
{
    Proxy myProxy;

    /* getting the file of configuaration */
    myProxy.setConfigFilePath((argc > 1) ? std::string(argv[1]) : DEFAULT_CONFIG_FILEPATH);

    /* setting the configuarations */
    std::cout << "Loading configuaration from: " << myProxy.getConfigFilePath() << std::endl;

    if (myProxy.loadConfiguaration() == Proxy_Error_t::NOT_OK)
    {
        std::cerr << "Unable to load configuaration!" << std::endl;
        std::cerr << "LOADING THE DEFAULT CONFIGUARATION!!" << std::endl;
    }

    try
    {
        /* Connect to the MQTT broker and wait till done */
        std::cout << "Connecting to server '" << myProxy.get_server_uri() << "'..." << std::flush;
        myProxy.connect()->wait();
        std::cout << "OK" << std::endl;

        myProxy.subscribe(); /* will be overloaded to match void input */

        while (true)
        {
            if (myProxy.getRxFalg() == Proxy_Flag_t::CARLA) /* received from carla */
            {
                /* parsing information */

                /* publish to RPIS */
                myProxy.publish(Proxy_Flag_t::RPIS);

                /* clear the flag */
            }
            else if (myProxy.getRxFalg() == Proxy_Flag_t::RPIS)
            {
                /* composing information */

                /* publish to CARLA */
                myProxy.publish(Proxy_Flag_t::CARLA);

                /* clear the flag */
            }
        }

        /* disconnect and wait till done */
        std::cout << "\nDisconnecting..." << std::flush;
        myProxy.disconnect()->wait();
        std::cout << "OK" << std::endl;
    }
    catch (const std::exception &e)
    {
        /* print the exception error */
        std::cerr << e.what() << '\n';

        return 1;
    }
}