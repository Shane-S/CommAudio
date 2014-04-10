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
LPFN_ACCEPTEX   AcceptPtr;

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
DWORD WINAPI Serve(VOID *pProps)
{
	DWORD           bytesRecvd;
	LPTransferProps props = (LPTransferProps)pProps;
	CHAR            out_buf[sizeof(DWORD)+((sizeof(sockaddr_in) + 16)* 2)] = { 0 };
	WSAOVERLAPPED   *ovr        = new WSAOVERLAPPED;
	WSAEVENT        hEvent      = CreateEvent(NULL, 0, 0, TEXT("AcceptExEvt"));
	LPSOCKADDR_IN   client		= (LPSOCKADDR_IN)malloc(sizeof(SOCKADDR_IN));
	SOCKET			listenSock	= props->socket;
	SOCKET			acceptSock  = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, 0);
	GUID            acceptID    = WSAID_ACCEPTEX;

	WSAIoctl(listenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &acceptID, sizeof(acceptID), &AcceptPtr, sizeof(AcceptPtr), &bytesRecvd,
		NULL, NULL);

	memset(ovr, 0, sizeof(WSAOVERLAPPED));
	ovr->hEvent = hEvent;

	listen(listenSock, 5);

	OverlappedAccept(listenSock, acceptSock, out_buf, sizeof(uint32_t), &bytesRecvd, ovr);
	WSAWaitForMultipleEvents(1, &ovr->hEvent, FALSE, INFINITE, TRUE);
	MessageBox(NULL, TEXT("Accepted client!"), TEXT("ACCEPT THIS BITCH"), MB_OK);
	//closesocket(props->socket);

	ServerCleanup(props);
	return 0;
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
 * Increments the number of packets received when a WSARecvFrom completes on a UDP socket.
 *
 * @param props Pointer to the TransferProps structure containing details about this transfer.
 * @return True if the WSARecvFrom succeeded, or false if something went wrong.
 */
BOOL ListenUDP(LPTransferProps props)
{
	DWORD flags = 0, error = 0;
	props->dwTimeout = INFINITE;

	error = WSAGetLastError();
	if (error && error != WSA_IO_PENDING)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("WSARecvFrom Error"), TEXT("WSARecvFrom encountered error %d"), error);
		props->dwTimeout = 0;
		return FALSE;
	}
	return TRUE;
}


/**
 * Starts an overlapped accept operation on the listen socket.
 *
 * @param listenSocket      A valid socket in listening mode.
 * @param newSock           The new socket on which to accept the connection.
 * @param lpNameLen         A buffer to receive the client's name's length when accept completes.
 * @param dwRecvDataLen     The size of lpNameLen.
 * @param lpdwBytesReceived Pointer to a DWORD which will receive the amount of data actually transferred.
 * @param lpOverlapped      The overlapped structure to be activated on accepting a connection.
 *
 * @return -1 on failure, 0 if the operation is still in progress, and 1 if a socket was accepted within
 *         the function call.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @data     April 3rd, 2014
 */
INT OverlappedAccept(SOCKET listenSocket, SOCKET newSock, PVOID lpNameLen, DWORD dwRecvDataLen, LPDWORD lpdwBytesReceived,
	                    LPOVERLAPPED lpOverlapped)
{
	errno_t error;
	if (!AcceptPtr(listenSocket, newSock, lpNameLen, dwRecvDataLen, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16,
		lpdwBytesReceived, lpOverlapped) && ((error = WSAGetLastError()) != WSA_IO_PENDING))
	{
		TCHAR message[128] = { 0 };
		_tcprintf_s(message, TEXT("%s, error number %d."), TEXT("Error accepting socket"), error);
		LogError(TEXT("OverlappedAccept"), message);
		return -1;
	}
	else if (error == WSA_IO_PENDING)
		return 0;

	return 1;
}
