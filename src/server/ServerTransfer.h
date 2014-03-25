/**
 * Declares all functions related to the server side of the transfer.
 * @file ServerTransfer.h
 */

#ifndef SERVER_TRANSFER_H
#define SERVER_TRANSFER_H

#include <WinSock2.h>
#include <Windows.h>
#include <time.h>
#include "../WinStorage.h"
#include "../Utils.h"

#define UDP_MAXPACKET	65000	/**< The maximum UDP datagram size */
#ifndef COMM_TIMEOUT		
	#define COMM_TIMEOUT 5000 /**< Time to wait before giving up on receiving more data */
#endif

// Initialise the server and prepare to serve the client
BOOL ServerInitSocket(LPTransferProps props);
DWORD WINAPI Serve(VOID *hwnd);

// Listen for connection attempts (TCP) and data (UDP)
BOOL ListenTCP(LPTransferProps props);
BOOL ListenUDP(LPTransferProps props);


// Completion routine prototypes
VOID CALLBACK UDPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK TCPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);

// Prepare server for next volley of packets
VOID ServerCleanup(LPTransferProps props);
#endif
