#include "Multicast.h"

extern HSTREAM streamBuffer2;

VOID CALLBACK MulticastSendComplete(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	ServerInfo *server = (ServerInfo *)lpOverlapped;
	SOCKADDR_IN multicastAddr;
	char streamDataBuffer[2048];
	int bytesRead;
	WSABUF wsaBuf;

	memset(&multicastAddr, 0, sizeof(SOCKADDR_IN));
	multicastAddr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	multicastAddr.sin_port = htons(MULTICAST_PORT);
	multicastAddr.sin_family = AF_INET;

	memset(&wsaBuf, 0, sizeof(WSABUF));
	
	bytesRead = BASS_ChannelGetData(streamBuffer2, streamDataBuffer, 2048);
	if (!bytesRead)
		return;

	wsaBuf.buf = streamDataBuffer;
	wsaBuf.len = bytesRead;
	WSASendTo(server->getUDPMulticast(), &wsaBuf, 1, NULL, 0, (const sockaddr *)&multicastAddr, sizeof(multicastAddr),
		(LPOVERLAPPED)server, MulticastSendComplete);
}
