#include "udp.hpp"
#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace UDP
{
    Client::Client(const char* hostIP)
    {
        mHostIP = (char*)hostIP;

        mSockFD = socket(AF_INET, SOCK_STREAM, 0);
        if (mSockFD == -1) {
            std::cerr << "Socket creation failed\n";       
            exit(-1);
        }

        bzero(&mServerAddr, sizeof(mServerAddr));

        // Assign IP and port
        mServerAddr.sin_family = AF_INET;
        mServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        mServerAddr.sin_port = 8080;

    }

    Client::~Client()
    {

    }

    void Client::Initialize()
    {
    }
}
