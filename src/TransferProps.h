#ifndef TRANSFER_PROPS_H
#define TRANSFER_PROPS_H

#include <WinSock2.h>
#include <Windows.h>
#include <cstring>

#define BUFSIZE      1024
#define PORTNUM      7000
#define COMM_TIMEOUT 5000

/**
 * Stores details necessary to transfer data between a server and client.
 *
 * @struct TransferProps
 */
typedef struct _TransferProps
{
	WSAOVERLAPPED	wsaOverlapped;		 /**< An overlapped structure for use with completion routines. */
	LPSOCKADDR_IN	paddr_in;			 /**< Pointer to a sockaddr_in struct which holds server/client address details. */
	SOCKET			socket;				 /**< A socket on which to send and/or receive. */
	DWORD			nSockType;			 /**< The type of socket (SOCK_STREAM or SOCK_DGRAM) */
	WSABUF			dataBuffer;			 /**< Holds data received from or to send to another host. */
	HANDLE			audioFile;			 /**< Name of a (user-specified) file to send or to receive into. */
	DWORD			dwTimeout;			 /**< The time to wait before giving up (used for servers waiting on UDP packets). */
	BOOL			transferCancelled;	 /**< Whether the remote user asked to cancel sending (the stream was paused, etc) */
} TransferProps, *LPTransferProps;


LPTransferProps CreateTransferProps();
VOID DestroyTransferProps(LPTransferProps props);

#endif