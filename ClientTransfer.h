#ifndef TRANSFER_H
#define TRANSFER_H

#include <WinSock2.h>
#include <Windows.h>
#include <Winternl.h>
#include <cstdio>
#include <cstring>
#include <time.h>
#include "WinStorage.h"
#include "Utils.h"

#ifndef COMM_TIMEOUT
	#define COMM_TIMEOUT 5000
#endif

BOOL ClientInitSocket(LPTransferProps props);
DWORD WINAPI ClientSendData(VOID *hwnd);
VOID CALLBACK UDPSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, 
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK TCPSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, 
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
BOOL LoadFile(LPWSABUF wsaBuf, const TCHAR *szFileName, char **buf, LPDWORD lpdwFileSize, LPTransferProps props);

#endif