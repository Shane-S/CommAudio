/**
 * Declares all functions related to the server side of the transfer.
 * @file ServerTransfer.h
 */

#ifndef SERVER_TRANSFER_H
#define SERVER_TRANSFER_H

#include <ws2tcpip.h>
#include <mswsock.h>
#include <conio.h>
#include "../TransferProps.h"
#include "../Utils.h"

#define UDP_MAXPACKET	65000	/**< The maximum UDP datagram size */

// Initialise the server and prepare to serve the client
BOOL ServerInitSocket(LPTransferProps props);
DWORD WINAPI Serve(VOID *hwnd);

// Listen for connection attempts (TCP) and data (UDP)
BOOL ListenTCP(LPTransferProps props);
BOOL ListenUDP(LPTransferProps props);


// Completion routine prototypes
VOID CALLBACK UDPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);

VOID CALLBACK ServerUniSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);

// Prepare server for next volley of packets
VOID ServerCleanup(LPTransferProps props);

// Wrapper for AcceptEx
SOCKET OverlappedAccept(SOCKET sListenSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, LPDWORD lpdwBytesReceived,
                        LPOVERLAPPED lpOverlapped);
VOID CALLBACK UnicastFileSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);
#endif
