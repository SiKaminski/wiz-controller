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
#include <jansson.h>

#include "globals.hpp"
#include "bulb.hpp"

#include "inactivity_timer.hpp"
#include "skutils/logger/logger.hpp"


namespace Wiz
{
    Controller::~Controller()
    {
        InitBulbs();
    }

    void Controller::InitBulbs()
    {
        // Reset the bulb array
        mBulbs.clear();
        mSearchResponses = SearchForBulbs();

        for (json_t* resp : mSearchResponses) {
            json_t* ipObject = json_object_get(resp, "ip");
            if (ipObject == nullptr) {
                Global::logger.Log(ERROR, "Unable to get device IP");
                continue;
            }

            json_t* portObject = json_object_get(resp, "port");
            if (portObject == nullptr) {
                Global::logger.Log(ERROR, "Unable to get device Port");
                continue;
            }

            std::string ip = json_string_value(ipObject);
            u_int16_t port = json_integer_value(portObject);

            Bulb* b = new Bulb(ip, port);
            mBulbs.push_back(b);
        }
    }

    std::vector<json_t*> Controller::SearchForBulbs() 
    {
        using namespace std::chrono;

        int sock = socket(AF_INET, SOCK_DGRAM|SOCK_NONBLOCK, 0);

        int broadcastEnable = 1;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

        u_int16_t port = Global::WIZ_UDP_BROADCAST_PORT;
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
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

                std::string devIP = inet_ntoa(sender.sin_addr);

                json_error_t error;
                json_t* root = json_loads(buf, 0, &error);

                if (!root) {
                    Global::logger.Log(ERROR, "Error parsing json: %s", error.text);
                    continue;
                }

                json_object_set_new(root, "ip", json_string(devIP.c_str()));
                json_object_set_new(root, "port", json_string(std::to_string(port).c_str()));
                jsonRepsonses.push_back(root);
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

    void Controller::ConfirmBulbChoices()
    {
        Global::logger.Log(WARNING, "ConfirmBulbChoices: Not implemented");
    }

    std::vector<Bulb*> Controller::FilterDevicesByModulePrefix(std::string prefix)
    {
        // tmp for now
        InitBulbs();
        
        return mBulbs;
    }

    // FIXME
    std::vector<Bulb*> Controller::FilterDevicesByHomeID(std::string homeId)
    {
        // mHomeId = homeId;
        Global::logger.Log(WARNING, "FilterDevicesByHomeID: Not implemented");
        return std::vector<Bulb*>();
    }

    // FIXME
    std::vector<Bulb*> Controller::FilterDevicesByRoomID(std::string roomId)
    {
        // mRoomId = roomId;
        Global::logger.Log(WARNING, "FilterDevicesByRoomID: Not implemented");
        return std::vector<Bulb*>();
    }
} // Namespace Wiz
