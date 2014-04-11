#include "ServerInfo.h"

using std::string;

ServerInfo::ServerInfo(const short tcpPort, const short udpPort)
: fakeOvr(), acceptOvr(), listenSocket(0), udpListenSock(0), dataBuffer(), clientList()
{
	WSAEVENT    hEvent = WSACreateEvent();
	SOCKADDR_IN paddr_in;
	memset(&paddr_in, 0, sizeof(SOCKADDR_IN));

	int yes = 1;
	if (hEvent == (HANDLE)-1)
		throw "Couldn't create event for accepting.";

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

	paddr_in.sin_port = htons(udpPort);
	udpListenSock = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (udpListenSock == INVALID_SOCKET)
		throw "ServerInfo::ServerInfo: Couldn't create udpListenSock.";

	BOOL fFlags = TRUE;
	setsockopt(udpListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&fFlags , sizeof(fFlags));
	if (bind(udpListenSock, (const sockaddr *)&paddr_in, sizeof(SOCKADDR_IN)) == -1)
	{
		int err = WSAGetLastError();
		throw "ServerInfo::ServerInfo: Couldn't bind udpListenSock.";
	}
}


HANDLE ServerInfo::getEvent() const
{
	return acceptOvr.hEvent;
}

SOCKET ServerInfo::getTCPListen() const
{
	return listenSocket;
}

SOCKET ServerInfo::getUDPListen() const
{
	return udpListenSock;
}

