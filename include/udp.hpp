#pragma once
#ifndef _UDP_CLIENT_HPP_
#define _UDP_CLIENT_HPP_

#include <string>

namespace UDP
{
    class Socket
    {
        public:
            Socket();
            ~Socket();

            std::string SendUdpCommand(
                    const std::string& msg,
                    const std::string& targetIP,
                    const u_int16_t port,
                    std::string& broadcastIP
                    );

        private:
            bool InitSocket();
            int mBroadcastSocket;
    };
}

#endif // _UDP_CLIENT_HPP_
