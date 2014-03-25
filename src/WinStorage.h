/**
 * Contains structures for transfer details and buffering data.
 *
 * @file WinStorage.h
 */

#ifndef WIN_STORAGE_H
#define WIN_STORAGE_H

#include <WinSock2.h>

#define GWLP_TRANSFERPROPS	0						// Offset value to access the transfer props pointer in wndExtra 
#define GWLP_HOSTMODE		sizeof(LPTransferProps)	// Offset value to access the host mode pointer in wndExtra
#define FILENAME_SIZE		512						// The max file name size (in bytes)
#define HOSTNAME_SIZE		128						// The max host name size (in bytes)

/**
 * Stores details necessary to transfer data between a server and client.
 *
 * @struct TransferProps
 */
typedef struct _TransferProps
{
	WSAOVERLAPPED	wsaOverlapped;				/**< An overlapped structure for use with completion routines. */
	LPSOCKADDR_IN	paddr_in;					/**< Pointer to a sockaddr_in struct which holds server/client address details. */
	SOCKET			socket;						/**< A socket on which to send and/or receive. */
	DWORD			nSockType;					/**< The type of socket (SOCK_STREAM or SOCK_DGRAM */
	TCHAR			szFileName[FILENAME_SIZE];	/**< Name of a (user-specified) file to send or to receive into. */
	DWORD			dwTimeout;					/**< The time to wait before giving up (used for servers waiting on UDP packets). */
	BOOL			transferCancelled;			/**< Whether the remote user asked to cancel sending (the stream was paused, etc) */
} TransferProps, *LPTransferProps;

#endif
