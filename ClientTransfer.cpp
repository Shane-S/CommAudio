#include "ClientTransfer.h"

/*
 * Post-condition:
 * If this returns true, the socket descriptor in props is (or was at one point) ready to send on.
 * If this returns false, the socket descriptor in props will be 0.
 */
/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ClientInitSocket
-- Febrary 1st, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: ClientInitSocket(LPTransferProps props)
--							LPTransferProps props:	Pointer to a TransferProps structure containing information about the
--													packet size and number, server IP/host name, etc.
--
-- RETURNS: False if the socket can't be created or the host name can't be resovled; true otherwise.
--
-- NOTES:
-- Preps a socket for sending. Resolves the host name if there is one and creates the socket, storing it in props->socket.
-- If the socket can't be created, props->socket is set to INVALID_SOCKET.
---------------------------------------------------------------------------------------------------------------------------*/
BOOL ClientInitSocket(LPTransferProps props)
{
	if (props->szHostName[0] != 0) // They specified a host name
	{
		struct hostent *hp;
		char		   name[HOSTNAME_SIZE];

		TCHAR_2_CHAR(name, props->szHostName, HOSTNAME_SIZE);
		hp = gethostbyname(name);

		if (hp == NULL)
		{
			props->socket = INVALID_SOCKET;
			switch (h_errno)
			{
			case HOST_NOT_FOUND:
				MessageBoxPrintf(MB_ICONERROR, TEXT("Host Not Found"),
					TEXT("Could not find host %s. Check connection settings."), props->szHostName);
				return FALSE;
			case TRY_AGAIN:
				MessageBoxPrintf(MB_ICONERROR, TEXT("Try Again"), TEXT("Unable to connect. Try again later."));
				return FALSE;
			case NO_RECOVERY:
				MessageBoxPrintf(MB_ICONERROR, TEXT("DNS Error"), TEXT("DNS error. Try again later."));
				return FALSE;
			case NO_ADDRESS:
				MessageBoxPrintf(MB_ICONERROR, TEXT("No IP"), TEXT("No IP address for host %s. Check connection settings."),
					props->szHostName);
				return FALSE;
			default:
				MessageBoxPrintf(MB_ICONERROR, TEXT("Unknown Error"), TEXT("Unkown error %d."), h_errno);
				return FALSE;
			}
		}
		props->paddr_in->sin_addr.s_addr = (ULONG)hp->h_addr_list[0];
	}
	
	if (props->paddr_in->sin_addr.s_addr != 0)
	{
		SOCKET s;
		if ((s = socket(PF_INET, props->nSockType, 0)) < 0)
		{
			MessageBox(NULL, TEXT("Could not create socket."), TEXT("No Socket"), MB_ICONERROR);
			props->socket = INVALID_SOCKET;
			return FALSE;
		}

		props->socket = s;
		return TRUE;
	}
	else
	{
		MessageBox(NULL, TEXT("No IP address or host name entered. Check connection settings."), TEXT("No Destination"),
			MB_ICONERROR);
		return FALSE;
	}
}

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ClientSendData
-- Febrary 1st, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: ClientSendData(VOID *params)
--							VOID *params: Handle to the main window cast as a VOID *.
--
-- RETURNS: -1 if the connection fails (TCP), the user-specified file couldn't be opened, or the buffer couldn't be allacted.
--			Returns 0 on successful sending. 
--
-- NOTES:
-- Sends either a chosen file (if there is one) or a specified number of packets of the specified size.
---------------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI ClientSendData(VOID *params)
{
	HWND			hwnd		= (HWND)params;
	LPTransferProps	props		= (LPTransferProps)GetWindowLongPtr(hwnd, GWLP_TRANSFERPROPS);
	SOCKET			s			= props->socket;
	BOOL			fUseFile	= FALSE;
	CHAR			*buf;
	DWORD			dwFileSize;

	if (props->szFileName[0] != 0)
	{
		HANDLE hFile;
		hFile = CreateFile(props->szHostName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == NULL)
		{
			MessageBoxPrintf(MB_ICONERROR, TEXT("Couldn't Open File"),
				TEXT("Could not open file %s. Please check the spelling or select a different file."), props->szFileName);
			return -1;
		}
		dwFileSize = GetFileSize(hFile, NULL);
		buf = (CHAR *)malloc(dwFileSize);
		ReadFile(hFile, buf, dwFileSize, NULL, NULL);
	}
	else
	{
		buf = (CHAR *)malloc(props->nPacketSize);
		memset(buf, 'a', props->nPacketSize);
	}

	if (buf == NULL)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("No Memory Allocated"),
			TEXT("Unable to allocate memory. Windows error: %d"), GetLastError());
		return -1;
	}

	if (props->nSockType == SOCK_DGRAM)
	{
		setsockopt(props->socket, SOL_SOCKET, SO_SNDBUF, buf, props->nPacketSize);
		for (int i = 0; i < props->nNumToSend; ++i)
		{
			if (sendto(props->socket, buf, props->nPacketSize, 0, (sockaddr *)props->paddr_in, sizeof(sockaddr)) == SOCKET_ERROR)
			{
				MessageBoxPrintf(MB_ICONERROR, TEXT("sendto error"), TEXT("sendto encountered error %d"), WSAGetLastError());
				break;
			}
			Sleep(1); // Have to sleep, else the send buffer will become overwhelmed
		}
	}
	else
	{
		if (connect(s, (sockaddr *)props->paddr_in, sizeof(sockaddr)) == -1)
		{
			MessageBox(NULL, TEXT("Could not connect. Check settings and try again."),
				TEXT("Could not connect to socket"), MB_ICONERROR);
			return -1;
		}

	}

	free(buf);
	closesocket(props->socket);
	props->socket = INVALID_SOCKET;
	return 0;
}
