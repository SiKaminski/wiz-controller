#pragma once
#ifndef _BULB_HPP_
#define _BULB_HPP_

#include <string>
#include <sys/types.h>

#include "udp.hpp"
#include "types.hpp"

struct BulbMeta 
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

class Bulb
{
    public:
        Bulb(BulbMeta meta);
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
        std::string SetRGB(Color color);        

        BulbMeta mMeta;
    private:
        std::string ParseResponse(std::string jsonStr, std::string addlParams = "");

        UDP::Socket     mSocket;
};

#endif // _BULB_HPP_
