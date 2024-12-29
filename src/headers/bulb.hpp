#pragma once
#ifndef _BULB_HPP_
#define _BULB_HPP_

#include <string>
#include <sys/types.h>

#include "udp.hpp"

class Bulb
{
    public:
        Bulb();
        ~Bulb();

        void SetDeviceIP(const std::string& ip);
        std::string GetDeviceIP();

        std::string Discover(const std::string& ip);

    private:
        std::string ParseResponse(std::string jsonStr, std::string addlParams = "");

        std::string mDevIP;
        u_int16_t mPort;
        UDP::Socket mSocket;
};

#endif // _BULB_HPP_
