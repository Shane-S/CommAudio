#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include <WinSock2.h>   //Windows sockets library
#include <WS2tcpip.h>   //Windows multicast structure
#include <string>       //C++ string library

class ClientNetwork
{
private:
    std::string     serverIP;
    int             portNo;
    SOCKET          serverSocket;

public:
    ClientNetwork(std::string serverIP, int portNo) : serverIP(serverIP), portNo(portNo) {};
    int connectToServer();
}

#endif