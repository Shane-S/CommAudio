#include "ServerTransfer.h"

/** 
 * Contains definitions of all functions for the server side.
 * 
 * This file defines all functions in ServerTransfer.h. It has a fairly long description, as one might
 * expect; it is, after all, a doxygen test file designed for performing some experiments with the tool.
 *
 * Here is a list of reasons why this is a good tool:
 * <ul>
 * <li>It supports "grouping", which allows the inclusion of multiple files in a group (good for modules)</li>
 * <li>It can use a variety of comment styles: namely, Qt, JavaDoc, and Doxygen's propriety style</li>
 * <li>It automatically links to other files when they're mentioned (see above)</li>
 * <li>It can use Markdown and LaTeX to do some pretty cool stuff</li>
 * <li>And on and on it goes</li>
 * </ul>
 *
 * Another nice thing about Doxygen is the ability to link elsewhere: http://www.github.com
 * @file ServerTransfer.cpp
 */

static DWORD	recvd	= 0;	// The number of bytes or packets received
static HANDLE	destFile = 0;	// A file to store the transferred data (if specified by the user)
/**
 * Initialises a TCP or UDP socket for use by the server.
 *
 * @author Shane Spoor
 * @param[in] props Pointer to the TransferProps struct containing socket info.
 * @return True if the socket was successfully initiliased, false otherwise.
 */
BOOL ServerInitSocket(LPTransferProps props)
{
	SOCKET s = WSASocket(AF_INET, props->nSockType, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	BOOL set = TRUE;

	props->paddr_in->sin_addr.s_addr = htonl(INADDR_ANY);

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
	props->socket = s;
	return TRUE;
}

/**
 * Listens for data/connection attempts, then receives the client's data.
 *
 * @author Shane Spoor
 * @designer Shane Spoor
 * @param[in] hwnd Handle to the window (cast as a VOID * for the thread prototype).
 * @return The thread's status on exit.
 */
DWORD WINAPI Serve(VOID *hwnd)
{
	BOOL			set		= TRUE;
	DWORD			flags	= 0;
	LPTransferProps props	= (LPTransferProps)GetWindowLongPtr((HWND)hwnd, GWLP_TRANSFERPROPS);
	LPSOCKADDR_IN   client  = (LPSOCKADDR_IN)malloc(sizeof(SOCKADDR_IN));
	DWORD			dwSleepRet;

	if (props->nSockType == SOCK_STREAM && !ListenTCP(props))
	{
		ServerCleanup(props);
		return 1;
	}
	else if (props->nSockType == SOCK_DGRAM && !ListenUDP(props))
	{
		ServerCleanup(props);
		return 2;
	}

	while (props->dwTimeout)
	{
		dwSleepRet = SleepEx(INFINITE, TRUE);
		if (dwSleepRet != WAIT_IO_COMPLETION)
			break; // We've lost some packets; just exit the loop
	}

	closesocket(props->socket);

	ServerCleanup(props);
	return 0;
}

/**
 * Increments the number of packets received when a WSARecvFrom completes on a UDP socket.
 *
 * @param dwErrorCode					The error code (if any) set while the WSARecvFrom happened.
 * @param dwNumberOfBytesTransferred	The number of bytes received.
 * @param lpOverlapped					Pointer to an overlapped I/O (not used since this is a completion routine).
 * @param dwFlags						The flags set on the socket.
 */
VOID CALLBACK UDPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	DWORD flags = 0;

	if (dwErrorCode != 0)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("UDP Recv Error"), TEXT("Error receiving UDP packet; error code %d"), dwErrorCode);
		props->dwTimeout = 0;
		return;
	}
	recvd += dwNumberOfBytesTransfered;	

	WSARecvFrom(props->socket, &props->dataBuffer, 1, NULL, &flags, NULL, NULL, (LPOVERLAPPED)props, UDPRecvCompletion);
}

/**
* Increments the number of bytes received when a WSARecv completes on a TCP socket.
*
* @param dwErrorCode					The error code (if any) set while the WSARecvFrom happened.
* @param dwNumberOfBytesTransferred:	The number of bytes received.
* @param lpOverlapped					Pointer to an overlapped I/O (not used since this is a completion routine).
* @param dwFlags						The flags set on the socket.
 */
VOID CALLBACK TCPRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props	= (LPTransferProps)lpOverlapped;
	DWORD			flags	= 0;

	if (dwErrorCode != 0)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("TCP Recv Error"), TEXT("Error receiving TCP packet; error code %d"), dwErrorCode);
		props->dwTimeout = 0;
		return;
	}

	recvd += dwNumberOfBytesTransfered;
	WSARecv(props->socket, &props->dataBuffer, 1, NULL, &flags, (LPOVERLAPPED)props, TCPRecvCompletion);
}

/**
 * Resets server properties in preparation for the next transfer.
 *
 * @param props Pointer to a TransferProps structure to free.
 */
VOID ServerCleanup(LPTransferProps props)
{
	recvd = 0;
	if (destFile)
		CloseHandle(destFile);
	destFile = 0;
}

/**
 * Listens for a TCP connection on a listening socket and accepts connections.
 *
 * @param props Pointer to the TransferProps structure containing information about this transfer.
 * @return False if something went wrong during the connection, true otherwise.
 */
BOOL ListenTCP(LPTransferProps props)
{
	SOCKET accept;
	DWORD flags = 0, error = 0;
	DWORD bytesRead;
	char *buf = (char *)malloc(BUFSIZE);

	if (listen(props->socket, 5) == SOCKET_ERROR)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("listen() Failed"), TEXT("listen() failed with socket error %d"), WSAGetLastError());
		return FALSE;
	}

	if ((accept = WSAAccept(props->socket, NULL, NULL, NULL, NULL)) == SOCKET_ERROR)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("WSAAccept Failed"), TEXT("WSAAccept() failed with socket error %d"), WSAGetLastError());
		return FALSE;
	}

	closesocket(props->socket); // close the listening socket
	props->socket = accept;		// assign the new socket to props->socket

	props->audioFile = CreateFile("C:\\Users\\Shane\\Music\\Zune\\Fleetwood Mac\\Rumours\\07 The Chain.mp3", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(props->audioFile, props->dataBuffer.buf, BUFSIZE, &bytesRead, NULL);
	props->dataBuffer.len = bytesRead;
	WSASend(props->socket, &props->dataBuffer, 1, NULL, 0, (LPOVERLAPPED)props, ServerUniSendCompletion);

	if (error && error != WSA_IO_PENDING)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("WSARecv Error"), TEXT("WSARecv encountered error %d"), error);
		props->dwTimeout = 0;
		return FALSE;
	}
	return TRUE;
}

/**
 * Increments the number of packets received when a WSARecvFrom completes on a UDP socket.
 *
 * @param props Pointer to the TransferProps structure containing details about this transfer.
 * @return True if the WSARecvFrom succeeded, or false if something went wrong.
 */
BOOL ListenUDP(LPTransferProps props)
{
	DWORD		flags = 0, error = 0;
	props->dwTimeout = INFINITE;

	WSARecvFrom(props->socket, &props->dataBuffer, 1, NULL, &flags, NULL, NULL, (LPOVERLAPPED)props,
		UDPRecvCompletion);

	error = WSAGetLastError();
	if (error && error != WSA_IO_PENDING)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("WSARecvFrom Error"), TEXT("WSARecvFrom encountered error %d"), error);
		props->dwTimeout = 0;
		return FALSE;
	}
	return TRUE;
}


