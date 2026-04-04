#include "wiz.hpp"

#include <bits/types/timer_t.h>
#include <chrono>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/time.h>

#include <string>


namespace Wiz
{
    Controller::Controller()
    {
        SearchForBulbs();
    }

    Controller::Controller(std::string devicePrefix)
    {
       SearchForBulbs(devicePrefix);
    }

    Controller::~Controller()
    {

    }

    std::vector<json_t*> Controller::SearchForBulbs() 
    {
        using namespace std::chrono;

        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        int broadcastEnable = 1;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(38899);
        addr.sin_addr.s_addr = inet_addr("255.255.255.255");

        const char* msg = "{\"method\":\"getSystemConfig\",\"params\":{}}";

        sendto(sock, msg, strlen(msg), 0, (sockaddr*)&addr, sizeof(addr));

        char buf[1024];
        sockaddr_in sender{};
        socklen_t senderLen = sizeof(sender);

        auto lastChangeTime = steady_clock::now();
        const auto timeout = seconds(2);


        std::vector<json_t*> jsonRepsonses;
        std::cout << "Waiting for response\n";
        while (true) {
            bool foundNewDevice = false;

            int len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&sender, &senderLen);
            if (len > 0) { 
                foundNewDevice = true;
                buf[len] = '\0';

                std::cout << "Found bulb at "
                          << inet_ntoa(sender.sin_addr)
                          << "\nResponse: "
                          << buf << "\n\n";

                json_t* resp = json_pack(buf);
                jsonRepsonses.push_back(resp);
            }

            if (foundNewDevice) {
                lastChangeTime = steady_clock::now();
            }

            // Check for inactivity
            auto now = steady_clock::now();
            auto elapsed = duration_cast<seconds>(now - lastChangeTime);
            std::cout << elapsed << "\n";

            // Done scanning
            if (elapsed >= timeout) {
                std::cout << "done scanning\n";
                break;
            }

            std::this_thread::sleep_for(milliseconds(200));
        }

        close(sock);
        return jsonRepsonses;
    }

    std::vector<json_t*> Controller::SearchForBulbs(std::string devicePrefix) 
    {
        return SearchForBulbs();
    }

} // Namespace Wiz
