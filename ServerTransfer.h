#ifndef SERVER_TRANSFER_H
#define SERVER_TRANSFER_H

#include <WinSock2.h>
#include <Windows.h>
#include <time.h>
#include "WinStorage.h"
#include "Utils.h"

#define UDP_MAXPACKET	65535
#define COMM_TIMEOUT	5000

BOOL ServerInitSocket(LPTransferProps props);
DWORD WINAPI Serve(VOID *hwnd);

// Completion routine prototypes
VOID CALLBACK UDPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK TCPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
#endif