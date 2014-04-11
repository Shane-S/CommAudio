#include "ServerTransfer.h"

using std::vector;
using std::string;

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

LPFN_ACCEPTEX             AcceptPtr;                // Pointer to the AcceptEx function
LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockaddrsPtr;  // Pointer to the GetAcceptExSockaddrs function
WSABUF                    nameWsaBuf;				// Holds a newly connected client's name
vector<ClientStruct>      clientList;				// 
int                       lastClient;

/**
 * Gets function pointers to the extended windows sockets functions allowing overlapped accept calls.
 *
 * Microsoft, in their infinite wisdom, decided that the extended socket functions would be retrieved at runtime
 * from the DLL. This isn't so bad in itself, but they could have made a dedicated function instead of shoe-horning
 * it into WSAIoctl. This function creates a fake socket, uses WSAIoctl to get the function pointers, then closes the
 * socket, having used it for precisely nothing.
 *
 * @return Whether the function succeeded.
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 10th, 2014
 */
BOOL ServerInitExtendedFuncs()
{
	GUID  acceptID    = WSAID_ACCEPTEX;
	GUID  getAcceptID = WSAID_GETACCEPTEXSOCKADDRS;
	DWORD bytesRecvd;
	SOCKET fakeSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, 0);

	if (WSAIoctl(fakeSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &acceptID, sizeof(acceptID), &AcceptPtr, sizeof(AcceptPtr), &bytesRecvd,
		NULL, NULL) == INVALID_SOCKET)
	{
		int err = GetLastError();
		MessageBox(NULL, TEXT("OH YEAHHH"), TEXT("Oh no! Ohh nooo! OH NO!"), MB_ICONERROR);
		return FALSE;
	}

	if(WSAIoctl(fakeSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &getAcceptID, sizeof(getAcceptID), &GetAcceptExSockaddrsPtr,
		sizeof(GetAcceptExSockaddrsPtr), &bytesRecvd, NULL, NULL) == INVALID_SOCKET)
	{
		MessageBox(NULL, TEXT("OH YEAHHH"), TEXT("Oh no! Ohh nooo! OH NO!"), MB_ICONERROR);
		return FALSE;
	}

	closesocket(fakeSock);
	return TRUE;
}

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
	props->paddr_in->sin_port = htons(7000);

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
	WSAOVERLAPPED   fakeOvr;
	WSAEVENT        hEvents[] = { CreateEvent(NULL, FALSE, FALSE, TEXT("AcceptExEvt")) };
	SOCKET			listenSock	= props->socket;
	SOCKET			acceptSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, WSA_FLAG_OVERLAPPED);
	DWORD			flagsAreSeriouslyStupid = 0;

	memset(ovr, 0, sizeof(WSAOVERLAPPED));
	memset(&fakeOvr, 0, sizeof(WSAOVERLAPPED));
	ovr->hEvent = hEvents[0];

	listen(listenSock, 5);

	OverlappedAccept(listenSock, acceptSock, out_buf, sizeof(uint32_t), &bytesRecvd, ovr);
	while (1)
	{
		int evt = WSAWaitForMultipleEvents(1, hEvents, FALSE, WSA_INFINITE, TRUE);
		if (evt == WSA_WAIT_EVENT_0) // Accepted a client
		{
			sockaddr_in clientAddr;
			uint32_t nameLen;
			ClientStruct newClient;

			free(nameWsaBuf.buf); // Free the buffer containing the previous name

			// Get the results from acceptex and assign them to a new client object in the list
			GetAcceptResults(out_buf, &nameLen, &clientAddr);
			newClient.client = new Client;

			memset(&newClient.fakeOvr, 0, sizeof(WSAOVERLAPPED));
			newClient.client->setAddr(clientAddr).setSock(acceptSock);

			clientList.push_back(newClient);

			nameWsaBuf.buf = (char *)malloc(nameLen);
			nameWsaBuf.len = nameLen;

			auto it = clientList.end();
			it--;

			WSARecv(acceptSock, &nameWsaBuf, 1, NULL, &flagsAreSeriouslyStupid, (LPOVERLAPPED)&(*it) , RecvNameCompletion);

			int err = GetLastError();
			acceptSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, 0);
			OverlappedAccept(listenSock, acceptSock, out_buf, sizeof(uint32_t), &bytesRecvd, ovr);
		}
	}

	ServerCleanup(props);
	return 0;
}


/**
 *
 */
VOID CALLBACK RecvNameCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped,
	DWORD dwFlags)
{
	ClientStruct *clnt = (ClientStruct *)lpOverlapped;
	Client *clientPtr = clnt->client;
	string newName(nameWsaBuf.buf, nameWsaBuf.len);
	
	clientPtr->setName(newName);
	fprintf(stderr, "Accepted client with address %s and name %s\n", inet_ntoa(clientPtr->getAddr().sin_addr), 
		clientPtr->getName().c_str());
}

/**
 * Resets server properties in preparation for the next transfer.
 *
 * @param props Pointer to a TransferProps structure to free.
 */
VOID ServerCleanup(LPTransferProps props)
{
	//recvd = 0;
	//if (destFile)
	//	CloseHandle(destFile);
	//destFile = 0;
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

/**
 * Gets the relevant results an AcceptEx call.
 *
 * Retrives the data sent by the client and the client's sockaddr_in from the result buffer.
 *
 * @param resultBuf  The buffer containing the results from AcceptEx.
 * @param nameLen    Receives the client's name's length.
 * @param clientInfo Receives sockaddr_in information for the client.
 * @return Whether the call succeeded (0 or 1).
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 * @date     April 10th, 2014
 */
INT GetAcceptResults(void *resultBuf, uint32_t *nameLen, sockaddr_in *clientInfo)
{
	sockaddr_in *clientAddr = NULL;
	sockaddr_in *serverAddr = NULL;
	uint8_t *something;
	int clientLen;
	int serverLen;
	int bufLen = sizeof(int)+((sizeof(SOCKADDR_IN)+16) * 2);

	memcpy(nameLen, resultBuf, sizeof(uint32_t)); // Copy the name length into the variable passed in
	GetAcceptExSockaddrsPtr(resultBuf,
							bufLen - ((sizeof(SOCKADDR_IN)+16) * 2),
							sizeof(sockaddr_in) + 16,
							sizeof(sockaddr_in) + 16, 
							(sockaddr **)&serverAddr, 
							&serverLen,
							(sockaddr **)&clientAddr, 
							&clientLen);
	something = (uint8_t *)resultBuf;
	memcpy(clientInfo, clientAddr, sizeof(sockaddr_in));
	return 1;
}
