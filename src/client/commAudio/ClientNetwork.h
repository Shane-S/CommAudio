#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include <WinSock2.h>   //Windows sockets library
#include <WS2tcpip.h>   //Windows multicast structure
#include <Windows.h>
#include <string>       //C++ string library
#include "ConnectionSettings.h"
#include "bass.h"

#define WM_SOCKET   (WM_USER + 1)

class ClientNetwork
{
private:
    ConnectionSettings  connectionSettings;
    SOCKET              serverTCPSocket;
    SOCKET              serverUDPSocket;
    WSADATA             stWSAData;
    struct sockaddr_in  serverUDPAddr;
    HWND                hwnd;

public:
    ClientNetwork() {}
    ClientNetwork(ConnectionSettings connectionSettings) : connectionSettings(connectionSettings) {};
    
    void initWinsock(); //Call WSAStartup
    void terminateWinSock();

    int connectToTCPServer();
    int initUDPClient();
    
    void sendAudioData(void *data, bool isTCP, bool isFile);
    void getAudioData(bool isTCP);
    void sendPing();

    SOCKET getTCPSocket() {return serverTCPSocket;}
    void setHWND(HWND hwnd_) {hwnd = hwnd_;}
    void setConnectionSettings(ConnectionSettings connectionSettings_) { connectionSettings = connectionSettings_; }

    void closeSocket(SOCKET socket);
};

#endif