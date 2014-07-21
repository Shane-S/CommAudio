#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <cstring>
#include <vector>
#include <memory>
#include "Client.h"
#include "Multicast.h"

struct ClientStruct;

class ServerInfo
{
	
public:
	WSAOVERLAPPED	fakeOvr;       /**< An overlapped structure to make completion routines happy. */
	WSAOVERLAPPED   acceptOvr;     /**< An overlapped structure for accepting connections. */

	ServerInfo(const short tcpPort, const short udpPort);

	SOCKET getTCPListen() const;
	SOCKET getUDPListen() const;
	SOCKET getUDPMulticast() const;
	HANDLE getEvent() const;

	void setTCPListen(SOCKET sock);
	void setUDPListen(SOCKET sock);

private:
	SOCKET		 	                listenSocket;  /**< A socket on which to listen for TCP connections. */
	SOCKET		  	                udpListenSock; /**< A socket on which to listen for UDP data. */
	SOCKET							udpMulticast;  /**< A socket for multicast sending. */
	WSABUF			                dataBuffer;    /**< Holds data received from or to send to another host. */
	std::vector<ClientStruct>       clientList;    /**< Holds the list of currently connected clients. */ 
};


#endif