#include "udp.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

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
        if (mBroadcastSocket < 0) {
            InitSocket();
        }
        
        // Set up a socket for the bulb using passed in IP arguments
        sockaddr_in ipAddr;
        memset(&ipAddr, 0, sizeof(ipAddr));
        ipAddr.sin_family = AF_INET;
        ipAddr.sin_addr.s_addr = inet_addr(targetIP.c_str());
        ipAddr.sin_port = htons(port);
        int msgLen = msg.length();
        socklen_t len = sizeof(ipAddr);

        if (sendto(mBroadcastSocket, msg.c_str(), msgLen, 0, (sockaddr*) &ipAddr, len) != msgLen) {
            perror("sendto");
            return "";
        }
        
        char resp[MAXLINE] = {};
        int n = recvfrom(mBroadcastSocket, (char*)resp, MAXLINE, MSG_WAITALL, (sockaddr*) &ipAddr, &len);
        if (n < 0) {
            perror("recvfrom");
            return resp; 
        }

        resp[n] = '\0';
        if (broadcastIP.empty()) {
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipAddr.sin_addr), str, INET6_ADDRSTRLEN);
            broadcastIP = str;
            printf("Broadcast IP: %s\n", broadcastIP.c_str());
        }

        return resp;
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

        printf("UDP Socket initialized\n");
        return true;
    }
}
