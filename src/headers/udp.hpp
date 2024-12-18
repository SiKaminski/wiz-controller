#pragma once
#ifndef _UDP_CLIENT_HPP_
#define _UDP_CLIENT_HPP_

#include <sys/socket.h>
#include <netinet/in.h>

namespace UDP
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
            sockaddr_in mServerAddr;
            sockaddr_in mCli;
    };
}

#endif // _UDP_CLIENT_HPP_
