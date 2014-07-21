/**
 * Declares all functions related to the server side of the transfer.
 * @file ServerTransfer.h
 */

#ifndef SERVER_TRANSFER_H
#define SERVER_TRANSFER_H

#include <list>
#include <vector>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <conio.h>
#include <cstdint>
#include "AudioLibrary.h"
#include "audioManagement.h"
#include "../Utils.h"

#define UDP_MAXPACKET	65000	/**< The maximum UDP datagram size */
#define BUFSIZE			2048

class ServerInfo;

// Initialise the server and prepare to serve the client
BOOL ServerInitExtendedFuncs();
DWORD WINAPI Serve();
int AddClient(char *outBuf, SOCKET acceptSock, ServerInfo *serve, DWORD *flags, DWORD *bytes, INT *len);

VOID CALLBACK ServerUniSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK RecvNameCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped,
	DWORD dwFlags);

// Wrappers for the extended functions (AcceptEx and GetAcceptexSockaddrs)
INT OverlappedAccept(SOCKET listenSocket, SOCKET newSock, PVOID lpNameLen, DWORD dwRecvDataLen, LPDWORD lpdwBytesReceived,
						LPOVERLAPPED lpOverlapped);
INT GetAcceptResults(void *resultBuf, uint32_t *nameLen, sockaddr_in *clientInfo);

VOID CALLBACK UnicastFileSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);
#endif
