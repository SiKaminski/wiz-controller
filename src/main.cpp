#include "bulb.hpp"

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>

#include "wiz.hpp"

constexpr int BULB_COUNT {1};

int main(int argc, char** argv)
{
    Wiz::Controller controller("ESP03");

    // while (1) {
    //     for (int i = 0; i < 255; i+=10) {
    //         bulbs[0].SetRGB(i, 0, 0);
    //     }

    //     for (int i = 0; i < 255; i+=10) {
    //         bulbs[0].SetRGB(0, i, 0);
    //     }

    //     for (int i = 0; i < 255; i+=10) {
    //         bulbs[0].SetRGB(0, 0, i);
    //     }
    // }

    return 0;
}
