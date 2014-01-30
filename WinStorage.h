#ifndef WIN_STORAGE_H
#define WIN_STORAGE_H

#include <windowsx.h>

#define GWLP_TRANSFERPROPS	0
#define GWLP_HOSTMODE		sizeof(LPTransferProps)
#define BUFSIZE				256
#define FILENAME_SIZE		512
#define HOSTNAME_SIZE		128

typedef struct _TransferProps
{
	LPSOCKADDR_IN	paddr_in;
	SOCKET			socket;
	DWORD			nSockType;
	TCHAR			szFileName[FILENAME_SIZE];
	TCHAR			szHostName[HOSTNAME_SIZE];
	DWORD			nPacketSize;
	DWORD			nNumToSend;
} TransferProps, *LPTransferProps;


#endif