#include "ServerTransfer.h"

// "Global" variables (used only in this file)
static SOCKET	listener;				// Listening socket
static DWORD	recvd	= 0;			// Either the number of bytes or packets received
static DWORD	udpTotal = 0;			// Total number of UDP packets expected 
static WSABUF	wsaBuf;					// Data buffer


DWORD WINAPI Serve(VOID *hwnd)
{
	BOOL set = TRUE;
	SOCKADDR_IN client;
	LPTransferProps props = (LPTransferProps)GetWindowLongPtr((HWND)hwnd, GWLP_TRANSFERPROPS);
	char buf[BUFSIZE];

	wsaBuf.buf = buf;
	wsaBuf.len = BUFSIZE;
	setsockopt(props->socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&set, sizeof(int));

	props->dwTimeout = INFINITE;

	if (props->nSockType == SOCK_STREAM)
	{
		// Run some TCP receive code
	}
	else
	{
		INT client_size = sizeof(client);
		DWORD flags = 0;
		while (1)
		{
			DWORD error, dwSleepRet;
			WSARecvFrom(props->socket, &wsaBuf, 1, NULL, &flags, (sockaddr *)&client, &client_size, (LPOVERLAPPED)props,
				UDPRecvCompletion);
			
			if ((error = WSAGetLastError()) == WSAENOTSOCK)
				break;
			
			dwSleepRet = SleepEx(props->dwTimeout, TRUE);
			if(dwSleepRet != WAIT_IO_COMPLETION)
				break;
		}
	}
	closesocket(props->socket);
	MessageBoxPrintf(MB_ICONERROR, TEXT("Received"), TEXT("Received %d packets."), recvd);
	DrawTextPrintf((HWND)hwnd, TEXT("Received %d packets."), recvd);
	
	recvd = 0;
	props->startTime = 0;
	props->endTime = 0;
	return 0;
}


BOOL ServerInitSocket(LPTransferProps props)
{
	SOCKET s = WSASocket(AF_INET, props->nSockType, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);

	props->paddr_in->sin_addr.s_addr = INADDR_ANY;

	if (s == INVALID_SOCKET)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("WSASocket Failed"), TEXT("Could not create socket, error %d"), WSAGetLastError());
		return FALSE;
	}

	if (bind(s, (sockaddr *)props->paddr_in, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("bind Failed"), TEXT("Could not bind socket, error %d"), WSAGetLastError());
		return FALSE;
	}
	DWORD error = WSAGetLastError();
	props->socket = s;
	return TRUE;
}


VOID CALLBACK UDPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	if (dwErrorCode != 0)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("UDP Recv Error"), TEXT("Error receiving UDP packet; error code %d"), dwErrorCode);
		closesocket(props->socket);
		return;
	}
	time(&props->endTime);
	++recvd;
	udpTotal = ((DWORD *)wsaBuf.buf)[0];
	if (recvd == udpTotal)
	{
		closesocket(props->socket);
		return;
	}

	// This is the first packet
	if (props->dwTimeout == INFINITE)
	{
		props->dwTimeout = COMM_TIMEOUT;
		time(&props->startTime);
	}
}


VOID CALLBACK TCPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{

}
