#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>

void tokenize(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

void commandCallback(const std_msgs::StringConstPtr& stringMsg)
{
    std::string data = stringMsg->data;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "torch_control");
    ros::NodeHandle nh;

    ros::Publisher ValveOn = nh.advertise<std_msgs::Empty>("ValveON", 1);
    ros::Publisher ValveOff = nh.advertise<std_msgs::Empty>("ValveOFF", 1);
    ros::Publisher ValveONOff = nh.advertise<std_msgs::UInt16>("ValveONOFF", 1);
    ros::Publisher PlasmaOn = nh.advertise<std_msgs::UInt16>("PlasmaON", 1);

    ros::Subscriber commandSub = nh.subscribe("torch_command", 10, commandCallback);

    while (ros::ok())
    {
        std::string command;
        ROS_INFO("Type command:");
        std::getline(std::cin, command);

        std::vector<std::string> parsedCommand;
        tokenize(command, ' ', parsedCommand);

        if (parsedCommand.size() != 2)
        {
            std::cout << "Invalid command" << std::endl;
            continue;
        }

        std_msgs::Empty msgEmpty;
        std_msgs::UInt16 msgUInt16;
        if (parsedCommand[0].compare("plasma") == 0)
        {
            msgUInt16.data = stoi(parsedCommand[1]);
            PlasmaOn.publish(msgUInt16);
            std::cout << "PlasmaOn" << std::endl;
        }
        else if (parsedCommand[0].compare("valve") == 0 && parsedCommand[1].compare("on") == 0)
        {
            ValveOn.publish(msgEmpty);
            std::cout << "ValveOn" << std::endl;
        } 
        else if (parsedCommand[0].compare("valve") == 0 && parsedCommand[1].compare("off") == 0)
        {
            ValveOff.publish(msgEmpty);
            std::cout << "ValveOff" << std::endl;
        } else if (parsedCommand[0].compare("fire") == 0 && parsedCommand[1].compare("on") == 0)
        {
            ValveOn.publish(msgEmpty);
            msgUInt16.data = 5;
            PlasmaOn.publish(msgUInt16);
            std::cout << "FireOn" << std::endl;
        } else if (parsedCommand[0].compare("fire") == 0 && parsedCommand[1].compare("off") == 0)
        {
            ValveOff.publish(msgEmpty);
            std::cout << "FireOff" << std::endl;
        }
        else {
            std::cout << "Unrecognized command" << std::endl;
        }

        ros::spinOnce();
    }
    
    // ros::spin();
    return 0;
}
