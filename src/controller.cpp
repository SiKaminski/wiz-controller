// FIXME: Fix the double call to the Bulb() contructor in InitBulbs()

#include "controller.hpp"

#include <bits/types/timer_t.h>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
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
    Controller::Controller()
    {
        mBulbs = std::vector<Bulb::Device*>();
        InitBulbs();
    }

    Controller::~Controller() { }

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

            json_t* resultRoot = json_object_get(resp, "result");
            if (resultRoot == nullptr) {
                Global::logger.Log(ERROR, "Unable to get device result information");
                continue;
            }

            json_t* moduleNameObject = json_object_get(resultRoot, "moduleName");
            if (moduleNameObject == nullptr) {
                Global::logger.Log(ERROR, "Unable to get module name information");
                continue;
            }

            // FIXME
            // json_t* homeIdObject = json_object_get(resultRoot, "homeId");
            // if (homeIdObject == nullptr) {
            //     Global::logger.Log(ERROR, "Unable to get device homeId"); 
            //     continue;
            // }

            Bulb::Meta meta = {
                .Mac = "",

                // FIXME
                // .HomeID = json_string_value(homeIdObject),
                .HomeID = "",
                
                .RoomID = "",
                .RGN = "",
                .ModuleName = json_string_value(moduleNameObject),
                .FwVersion = "",
                .GroupID = "",
                .Ping = "",
                .Ip = json_string_value(ipObject),
                .Port = (u_int16_t)json_integer_value(portObject),
                
            };

            Bulb::Device* b = new Bulb::Device(meta);
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
                json_object_set_new(root, "port", json_integer(port));
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

    void Controller::ToggleLights()
    {
        for(Bulb::Device* b : mBulbs) {
            b->ToggleLight(false);
        }
    }

    void Controller::SetColor(Color color)
    {
        for (Bulb::Device* b : mBulbs) {
            b->SetRGB(color);
        }
    }

    void Controller::SetBrightness(ushort brightness)
    {
        for (Bulb::Device*b : mBulbs) {
            std::string res = b->SetBrightness(brightness);
            // if (res = Global::ERR_BRIGHTNESS_OUT_OF_BOUNDS)
        }
    }


    std::vector<Bulb::Device*> Controller::FilterDevicesByModulePrefix(std::string prefix)
    {
        std::vector<Bulb::Device*> filtered;
        for(Bulb::Device* b : mBulbs) {
            std::string moduleName = b->mMeta.ModuleName;
            if (moduleName.contains(prefix)) {
                filtered.push_back(b);
            }
        }

        mBulbs = filtered;
        return filtered;
    }

    // FIXME
    std::vector<Bulb::Device*> Controller::FilterDevicesByHomeID(std::string homeId)
    {
        // mHomeId = homeId;
        Global::logger.Log(WARNING, "FilterDevicesByHomeID: Not implemented");
        return std::vector<Bulb::Device*>();
    }

    // FIXME
    std::vector<Bulb::Device*> Controller::FilterDevicesByRoomID(std::string roomId)
    {
        // mRoomId = roomId;
        Global::logger.Log(WARNING, "FilterDevicesByRoomID: Not implemented");
        return std::vector<Bulb::Device*>();
    }
} // Namespace Wiz
