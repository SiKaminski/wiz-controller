#include "wiz.hpp"

#include <bits/types/timer_t.h>
#include <ctime>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
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

        std::cout << "Waiting for response\n";

        std::vector<json_t*> jsonRepsonses;
        bool scanning = true;
        while (scanning) {
            int len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&sender, &senderLen);
            if (len > 0) { 
                buf[len] = '\0';

                json_t* resp = json_pack(buf);
                jsonRepsonses.push_back(resp);

                std::cout << "Found bulb at "
                          << inet_ntoa(sender.sin_addr)
                          << "\nResponse: "
                          << buf << "\n\n";
            }
            scanning = false;
        }

        close(sock);
        return jsonRepsonses;
    }

    std::vector<json_t*> Controller::SearchForBulbs(std::string devicePrefix) 
    {
        return SearchForBulbs();
    }

} // Namespace Wiz
