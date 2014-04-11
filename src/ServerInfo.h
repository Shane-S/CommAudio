#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <WinSock2.h>
#include <Windows.h>
#include <cstring>
#include <vector>
#include <memory>

class ServerInfo
{
	
public:
	WSAOVERLAPPED	fakeOvr;		     /**< An overlapped structure to make completion routines happy. */

	ServerInfo(const short tcpPort, const short udpPort);

	SOCKET getTCPListen() const;
	SOCKET getUDPListen() const;

private:
	WSAOVERLAPPED                   acceptOvr;     /**< An overlapped structure for accepting connections. */
	std::unique_ptr<SOCKADDR_IN>	paddr_in;	   /**< Pointer to a sockaddr_in struct which holds server/client address details. */
	SOCKET		 	                listenSocket;  /**< A socket on which to listen for TCP connections. */
	SOCKET		  	                udpListenSock; /**< A socket on which to listen for UDP data. */
	WSABUF			                dataBuffer;    /**< Holds data received from or to send to another host. */
	std::vector<ClientStruct>       clientList;    /**< Holds the list of currently connected clients. */ 
};


#endif