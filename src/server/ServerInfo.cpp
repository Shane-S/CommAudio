#include "ServerInfo.h"

using std::string;

/**
 * Initialises an object to hold the list of clients, listen sockets, and multicast sending socket.
 *
 * @param tcpPort The port on which to bind the TCP listen socket.
 * @param udpPort The port on which to bind the UDP listen socket.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 11th, 2014
 */
ServerInfo::ServerInfo(const short tcpPort, const short udpPort)
: fakeOvr(), acceptOvr(), listenSocket(0), udpListenSock(0), udpMulticast(0), dataBuffer(), clientList()
{
	WSAEVENT    hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("AcceptEx"));
	SOCKADDR_IN paddr_in;
	struct ip_mreq stMreq;

	memset(&paddr_in, 0, sizeof(SOCKADDR_IN));

	int yes = 1;
	if (hEvent == (HANDLE)-1)
		throw "ServerInfo::ServerInfo: Couldn't create event for accepting.";

	acceptOvr.hEvent = hEvent;

	paddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	paddr_in.sin_family      = AF_INET;
	paddr_in.sin_port        = htons(tcpPort);

	memset(&fakeOvr, 0, sizeof(WSAOVERLAPPED));
	dataBuffer.buf = (char *)malloc(sizeof(uint32_t));
	if (!dataBuffer.buf)
		throw "ServerInfo::ServerInfo: Couldn't allocate space for dataBuffer.";

	dataBuffer.len = sizeof(uint32_t);

	listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
		throw "ServerInfo::ServerInfo: Couldn't create listenSocket.";

	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)yes, sizeof(int));
	if (bind(listenSocket, (const sockaddr *)&paddr_in, sizeof(SOCKADDR_IN)) == -1)
		throw "ServerInfo::ServerInfo: Couldn't bind listenSocket.";

	if (listen(listenSocket, 5) == SOCKET_ERROR)
		throw "ServerInfo::ServerInfo: Couldn't start listening on listenSocket.";

	paddr_in.sin_port = htons(udpPort);
	udpListenSock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (udpListenSock == INVALID_SOCKET)
		throw "ServerInfo::ServerInfo: Couldn't create udpListenSock.";

	BOOL fFlags = TRUE;
	setsockopt(udpListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&fFlags , sizeof(fFlags));
	if (bind(udpListenSock, (const sockaddr *)&paddr_in, sizeof(SOCKADDR_IN)) == -1)
		throw "ServerInfo::ServerInfo: Couldn't bind udpListenSock.";

	udpMulticast = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (udpMulticast == INVALID_SOCKET)
		throw "ServerInfo::ServerInfo: Couldn't make udpMulticast.";

	paddr_in.sin_port        = 0;
	if (bind(udpMulticast, (struct sockaddr*) &paddr_in, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		throw "ServerInfo::ServerInfo: Coudln't bind udpMulticast socket.";

	stMreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	if (setsockopt(udpMulticast, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq)) == SOCKET_ERROR)
		throw "ServerInfo::ServerInfo: Coudn't join multicast group.";
}

/**
 * Gets the handle for the AcceptEx event.
 *
 * @return The handle to the AcceptEx event.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 11th, 2014
 */
HANDLE ServerInfo::getEvent() const
{
	return acceptOvr.hEvent;
}

/**
 * Gets the TCP socket that's set to listening mode.
 *
 * @return The TCP socket that's listening.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 11th, 2014
 */
SOCKET ServerInfo::getTCPListen() const
{
	return listenSocket;
}

/**
 * Gets the UDP socket used for listening.
 *
 * @return The UDP socket that's listening for incoming information.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 11th, 2014
 */
SOCKET ServerInfo::getUDPListen() const
{
	return udpListenSock;
}

/**
 * Gets the UDP socket used for multicasting.
 *
 * @return The UDP socket that's multicasting.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 11th, 2014
 */
SOCKET ServerInfo::getUDPMulticast() const
{
	return udpMulticast;
}
