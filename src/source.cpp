/* 
 * For now I am going to test out finding discoverable internet devices
 * in a kind of 'hacky' solution because this project is long overdue
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
// #include <signal.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>

#include <pcap/pcap.h>

// Packet constants
// ping packet size
#define PING_PKT_S 64

// Automatic port number
#define PORT_NO 0

// Timeout delay for receiving packets in seconds
#define REVC_TIMEOUT 1
#define PING_SLEEP_RATE 1000000

// Perform DNS lookup
char* DnsLookup(char* addrHost, sockaddr_in* addrConn)
{
    hostent* hostEnt;
    char* ip = (char*)malloc(NI_MAXHOST * sizeof(char));
    
    // No ip found for hostname
    if ((hostEnt = gethostbyname(addrHost)) == NULL)
        return NULL;

    // Fill address struct
    strcpy(ip, inet_ntoa(*(in_addr*)hostEnt->h_addr));

    (*addrConn).sin_family = hostEnt->h_addrtype;
    (*addrConn).sin_port = htons(PORT_NO);
    (*addrConn).sin_addr.s_addr = *(long*)hostEnt->h_addr;

    return ip;
}

// Resolve the reverse lookup of the hostname
char* ReverseDnsLookup(char* ipAddr)
{
    sockaddr_in tmpAddr;
    socklen_t len;
    char buf[NI_MAXHOST];
    char* retBuf;

    tmpAddr.sin_family = AF_INET;
    tmpAddr.sin_addr.s_addr = inet_addr(ipAddr);
    len = sizeof(sockaddr_in);
    
    // Not able to resolve reverse lookup of hostname
    if (getnameinfo((sockaddr*)&tmpAddr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
        return NULL;

    retBuf = (char*)malloc((strlen(buf) + 1) * sizeof(char));
    strcpy(retBuf, buf);
    return retBuf;
}

int main(int argc, char** argv)
{

    /*
     * This code for now is kind of useless but later when I want to scan
     * automatically for led bulbs it might be a good starting point
     */
    // FILE* pin = popen("nmap -sn 123 192.168.1.0/24","r");
    // char line[1024];
    // if (pin) {
    //     while (!feof(pin)) {
    //         fgets(line, 100, pin);
    //         printf("%s", line);
    //     }

    //     pclose(pin);
    // } ;
    return 0;
}
