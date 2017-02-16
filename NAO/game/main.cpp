#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <string>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>

#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alerror/alerror.h>
#include <alcommon/alproxy.h>
#include <alproxies/almemoryproxy.h>
#include <alvalue/alvalue.h>

#include <treasureHunt.hpp>
#include <ledAuxiliarFunctions.hpp>
#include <motionNAOqiFunction.hpp>

void parseOpt(std::string *naoBrokerIP, int *naoBrokerPort, int argc, char* argv[])
{
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help mesages.")
        ("pip", po::value<std::string>(naoBrokerIP)->default_value("127.0.0.1"), "IP of the parent broker. Default: 127.0.0.1")
        ("pport", po::value<int>(naoBrokerPort)->default_value(9559),"Port of the parent broker. Default: 9559");
    po::positional_options_description positionalOptions;
    po::variables_map vm; // Map containing all the options with their values
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), vm);

        if (vm.count("help"))
        {
            std::cout << "Treasure Hunt Robot"                                   << std::endl;
            std::cout << desc                                                      << std::endl;

            exit(0);
        }

        po::notify(vm);
    }
    catch(po::error &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << desc << std::endl;
        exit(1);
    }
}

// ==========================================================================

int main(int argc, char **argv)
{
    std::string parentBrokerIP;
    int parentBrokerPort;


    setlocale(LC_NUMERIC, "C"); // Need this to for SOAP serialization of floats to work

    parseOpt( &parentBrokerIP, &parentBrokerPort, argc, argv);

    std::cout << "parse created..." << std::endl;
    parentBrokerPort = 9559;
    // A broker needs a name, an IP and a port:
    const std::string brokerName = "mybroker";
    // FIXME: would be a good idea to look for a free port first
    int brokerPort = 0;
    // listen port of the broker (here an anything)
    const std::string brokerIp = "";

    // Create your own broker
    boost::shared_ptr<AL::ALBroker> broker;
    try
    {
        broker = AL::ALBroker::createBroker(
            brokerName,
            brokerIp,
            brokerPort,
            parentBrokerIP,
            parentBrokerPort,
            0    // you can pass various options for the broker creation,
                 // but default is fine
        );
    }
    catch(const AL::ALError& e)
    {
        std::cerr << "Fail to connect broker to: "
                  << parentBrokerIP
                  << ":"
                  << parentBrokerPort
                  << std::endl
                  << e.what()
                  << std::endl;

        AL::ALBrokerManager::getInstance()->killAllBroker();
        AL::ALBrokerManager::kill();

        return 1;
    }


    LEDAux::changeColor(0, 0, 0);
    TreasureRobot robot;

    int currentState, flagStop = true;
    currentState = INITIAL;

    while(flagStop)
    {
        switch(currentState)
        {
            case INITIAL :
                currentState = robot.initial();
                sleep(5);
                break;
            case LOOKING :
                currentState = robot.looking();
                break;
            case BALL_FOUND :
                currentState = robot.ballFound();
                break;
            case WAITING_MESSAGE :
                currentState = robot.waitMessage();
                break;
            case MESSAGE_Q :
                currentState = robot.quente();
                break;
            case MESSAGE_F :
                currentState = robot.frio();
                break;
            case GOAL :
                currentState = robot.goal();
                break;

        }
    }


    //robot.manageConnection(8001);


    return(0);
}
