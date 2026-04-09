#pragma once
#ifndef _BULB_HPP_
#define _BULB_HPP_

#include <string>
#include <sys/types.h>
#include <jansson.h>

#include "udp.hpp"
#include "types.hpp"

namespace Wiz::Bulb
{
    struct State
    {
        Color*  color;
        ushort  brightness;

        json_t* ConvertToJson();
    };

    struct Meta 
    {
        std::string Mac;
        std::string HomeID;
        std::string RoomID;
        std::string RGN;
        std::string ModuleName;
        std::string FwVersion;
        std::string GroupID;
        std::string Ping;
        std::string Ip;
        u_int16_t Port;
    };

    class Device
    {
        public:
            Device(Meta meta);
            ~Device();

            void SetDeviceIP(const std::string& ip);
            std::string GetDeviceIP();

            std::string Discover(const std::string& ip);
            std::string GetStatus();
            std::string GetDeviceInfo();
            std::string GetWifiConfig();
            std::string GetSystemConfig();
            std::string GetUserConfig();

            std::string ToggleLight(bool state);
            std::string SetBrightness(int brightness);
            std::string SetRGB(Color color);        

            json_t* StoreState();

            Meta mMeta;
        private:
            std::string ParseResponse(std::string jsonStr, std::string addlParams = "");

            UDP::Socket mSocket;
            State mState;
    };
} // namespace Wiz::Bulb

#endif // _BULB_HPP_
