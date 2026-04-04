#include "bulb.hpp"

// #include <asm-generic/socket.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>

constexpr int BULB_COUNT {1};

// int main()
// {
//     int sock = socket(AF_INET, SOCK_DGRAM, 0);
// 
//     int broadcastEnable = 1;
//     setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
// 
//     sockaddr_in addr{};
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(38899);
//     addr.sin_addr.s_addr = inet_addr("255.255.255.255");
// 
//     const char* msg = "{\"method\":\"getSystemConfig\",\"params\":{}}";
// 
//     sendto(sock, msg, strlen(msg), 0, (sockaddr*)&addr, sizeof(addr));
// 
//     char buf[1024];
//     sockaddr_in sender{};
//     socklen_t senderLen = sizeof(sender);
// 
//     std::cout << "Waiting for response\n";
// 
//     while (true) {
//         int len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&sender, &senderLen);
//         if (len > 0) { 
//             buf[len] = '\0';
// 
//             std::cout << "Found bulb at "
//                       << inet_ntoa(sender.sin_addr)
//                       << "\nResponse: "
//                       << buf << "\n\n";
//         }
// 
//     }
// 
//     close(sock);
// }

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

    bulbs[0].SetDeviceIP("192.168.1.24");
    // bulbs[1].SetDeviceIP("192.168.1.34");
    // bulbs[2].SetDeviceIP("192.168.1.35");
    // bulbs[3].SetDeviceIP("192.168.1.36");

    // Bulb bulbs[3];

    // bulbs[0].SetDeviceIP("192.168.1.19");
    // bulbs[1].SetDeviceIP("192.168.1.24");
    // bulbs[2].SetDeviceIP("192.168.1.25");


    bulbs[0].ToggleLight(true);
    bulbs[0].SetBrightness(100);

    while (1) {
        for (int i = 0; i < 255; i+=10) {
            bulbs[0].SetRGB(i, 0, 0);
        }

        for (int i = 0; i < 255; i+=10) {
            bulbs[0].SetRGB(0, i, 0);
        }

        for (int i = 0; i < 255; i+=10) {
            bulbs[0].SetRGB(0, 0, i);
        }
    }

    return 0;
}
