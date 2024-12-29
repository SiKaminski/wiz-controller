#include "udp.hpp"

#include <arpa/inet.h>

constexpr int UDP_REQUEST_TIMEOUT {2};
constexpr int MAXLINE {4096};

namespace UDP
{
    Socket::Socket()
    {
        InitSocket();
    }

    Socket::~Socket()
    {

    }

    std::string Socket::SendUdpCommand(const std::string& msg, 
            const std::string& targetIP, const u_int16_t port, 
            std::string& broadcastIP) 
    {
        
    }

    bool Socket::InitSocket()
    {
        // Create socket
        mBroadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (mBroadcastSocket < 0) {
            perror("socket");
            return false;
        }

        int permission = 1;
        if (setsockopt(mBroadcastSocket, SOL_SOCKET, SO_BROADCAST, (void*) &permission, sizeof(permission)) < 0) {
            perror("setsockopt: SO_BROADCAST");
            return false;
        }

        timeval tv;
        tv.tv_sec = UDP_REQUEST_TIMEOUT;
        tv.tv_usec = 0;
        if (setsockopt(mBroadcastSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
            perror("setsockopt: SO_RCVTIMEO");
            return false;
        }

        printf("UDP Socket initialized");
        return true;
    }
}
