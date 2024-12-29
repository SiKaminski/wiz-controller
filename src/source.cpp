#include "bulb.hpp"

#include <string>
#include <stdio.h>

int main(int argc, char** argv)
{
    Bulb bulb;

    /* 
        For now I am going to hardcode one of these ip addresses
        that returned as WIZ devices after running the following command
        nmap -p 38899 -sU 192.168.1.0/24

        192.168.1.34
        192.168.1.35
        192.168.1.36
        192.168.1.46
    */
    std::string devIP = "192.168.1.34";
    bulb.SetDeviceIP(devIP);

    devIP = bulb.GetDeviceIP();
    printf("Device IP: %s\n", devIP.c_str());

    bulb.Discover(devIP);

    return 0;
}
