#include "bulb.hpp"

#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>

constexpr int BULB_COUNT {4};

int main(int argc, char** argv)
{
    // This is a bad way of doing this
    // Eventually I would like to find the leds automatically and ask the
    // user if the ip addresses look correct
    //
    // For now I know what the IP addresses of the bulbs are so I am going to
    // do this simply to continue working on the main functionality of the
    // program
    Bulb bulbs[BULB_COUNT];

    bulbs[0].SetDeviceIP("192.168.1.33");
    bulbs[1].SetDeviceIP("192.168.1.34");
    bulbs[2].SetDeviceIP("192.168.1.35");
    bulbs[3].SetDeviceIP("192.168.1.36");

    for (int i = 0; i < BULB_COUNT; i++) {
        bulbs[i].SetBrightness(50);
    }

    return 0;
}
