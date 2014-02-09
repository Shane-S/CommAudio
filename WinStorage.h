#ifndef WIN_STORAGE_H
#define WIN_STORAGE_H

#include <windowsx.h>
#include <time.h>

#define GWLP_TRANSFERPROPS	0
#define GWLP_HOSTMODE		sizeof (LPTransferProps)
#define FILENAME_SIZE		512
#define HOSTNAME_SIZE		128

typedef struct _TransferProps
{
	WSAOVERLAPPED	wsaOverlapped;
	LPSOCKADDR_IN	paddr_in;
	SOCKET			socket;
	DWORD			nSockType;
	TCHAR			szFileName[FILENAME_SIZE];
	TCHAR			szHostName[HOSTNAME_SIZE];
	DWORD			nPacketSize;
	DWORD			nNumToSend;
	time_t			startTime;
	time_t			endTime;
	DWORD			dwTimeout;
} TransferProps, *LPTransferProps;

#endif