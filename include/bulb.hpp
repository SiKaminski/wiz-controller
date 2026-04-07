#pragma once
#ifndef _BULB_HPP_
#define _BULB_HPP_

#include <string>
#include <sys/types.h>

#include "udp.hpp"

class Bulb
{
    public:
        Bulb(std::string ip, u_int16_t port);
        ~Bulb();

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
        std::string SetRGB(ushort r, ushort g, ushort b);        

    private:
        std::string ParseResponse(std::string jsonStr, std::string addlParams = "");
        bool ColorInBound(ushort val);

        UDP::Socket     mSocket;
        std::string     mDevIP;
        u_int16_t       mPort;
};

#endif // _BULB_HPP_
