#include "wiz.hpp"

#include <bits/types/timer_t.h>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/time.h>
#include <string>

#include "inactivity_timer.hpp"
#include "globals.hpp"
#include "skutils/logger/logger.hpp"


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

        int sock = socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, 0);

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

        std::vector<json_t*> jsonRepsonses;
        Global::logger.Log(INFO, "Waiting for response");
        InactivityTimer timer(seconds(2));

        while (true) {
            bool foundNewDevice = false;

            int len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&sender, &senderLen);
            if (len > 0) { 
                foundNewDevice = true;
                buf[len] = '\0';

                Global::logger.Log(TRACE, "Found bulb at %s\nResponse: %s",
                        inet_ntoa(sender.sin_addr),
                        buf);

                json_t* resp = json_pack(buf);
                jsonRepsonses.push_back(resp);
            }

            if (foundNewDevice) {
                timer.reset();
            } else if (timer.expired()) {
                Global::logger.Log(SUCCESS, "Done Scanning");
                break;
            }

            std::this_thread::sleep_for(milliseconds(200));
        }

        close(sock);
        return jsonRepsonses;
    }

    std::vector<json_t*> Controller::SearchForBulbs(std::string devicePrefix) 
    {
        std::vector<json_t*> allBulbs = SearchForBulbs();

        for (auto bulb : allBulbs) {
            Global::logger.Log(INFO, "Bulb: %s", bulb);
        }

        return allBulbs;
    }

} // Namespace Wiz
