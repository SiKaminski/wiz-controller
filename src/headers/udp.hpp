#pragma once
#ifndef _UDP_CLIENT_HPP_
#define _UDP_CLIENT_HPP_

#include <sys/socket.h>

namespace udp
{
    class Client
    {
        public:
            Client(const char* hostIP);
            ~Client();

            void Initialize();

        private:
            char* mHostIP;
            int mSockFD;
            int mConnFD;


    };
}

#endif // _UDP_CLIENT_HPP_
