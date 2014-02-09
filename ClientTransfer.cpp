#include "ClientTransfer.h"

static DWORD	sent = 0;				// Either the number of bytes or packets sent
static time_t	startTime, endTime;		// Start and end timestamps for the transfer

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
		if ((s = WSASocket(PF_INET, props->nSockType, 0, NULL, NULL, WSA_FLAG_OVERLAPPED)) < 0)
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
	LPTransferProps props		= (LPTransferProps)GetWindowLongPtr(hwnd, GWLP_TRANSFERPROPS);
	SOCKET			s			= props->socket;
	BOOL			bUseFile	= FALSE;
	WSABUF			wsaBuf;
	CHAR			*buf;
	DWORD			dwFileSize	= 0;

	if (props->szFileName[0] != 0)
	{
		if (!LoadFile(&wsaBuf, props->szFileName, &buf, &dwFileSize, props))
			return 1;
	}
	else
	{
		CHAR *size = (CHAR *)&props->nNumToSend;
		buf = (CHAR *)malloc(props->nPacketSize);
		memset(buf, 'a', props->nPacketSize);
		wsaBuf.len = props->nPacketSize;

		// Copy the number of packets to send into the buffer
		for (int i = 0; i < sizeof(DWORD); ++i)
			buf[i] = size[i];
	}

	if (buf == NULL)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("No Memory Allocated"),
			TEXT("Unable to allocate memory. Windows error: %d"), GetLastError());
		return -1;
	}
	wsaBuf.buf = buf;

	if (props->nSockType == SOCK_DGRAM)
	{
		setsockopt(props->socket, SOL_SOCKET, SO_SNDBUF, buf, props->nPacketSize);
		for (int i = 0; i < props->nNumToSend; ++i)
		{
			DWORD error;
			if(WSASendTo(props->socket, &wsaBuf, 1, NULL, 0, (sockaddr *)props->paddr_in, sizeof(sockaddr), 
				(LPOVERLAPPED)props, UDPSendCompletion) == SOCKET_ERROR && (error = WSAGetLastError()) != WSA_IO_PENDING)
			{
				if (error = WSAENOTSOCK)
					break;
			}
			SleepEx(INFINITE, TRUE);
		}
	}
	else
	{
		if (WSAConnect(s, (sockaddr *)props->paddr_in, sizeof(sockaddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
		{
			MessageBox(NULL, TEXT("Could not connect. Check settings and try again."),
				TEXT("Could not connect to socket"), MB_ICONERROR);
		}
		else
		{
			for (int i = 0; i < props->nNumToSend; ++i)
			{
				DWORD error;
				if (WSASend(s, &wsaBuf, 1, NULL, 0, (LPOVERLAPPED)props, TCPSendCompletion) == SOCKET_ERROR
					&& (error = WSAGetLastError()) != WSA_IO_PENDING)
				{
					if (error = WSAENOTSOCK)
						break;
				}

			}
		}
	}

	free(buf);
	closesocket(props->socket);
	props->socket = INVALID_SOCKET;
	return 0;
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
VOID CALLBACK UDPSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	if (dwErrorCode != 0)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("sendto error"), TEXT("WSASendTo encountered error %d"), dwErrorCode);
		closesocket(props->socket);
		return;
	}
	++sent;
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
VOID CALLBACK TCPSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	if (dwErrorCode != 0)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("send error"), TEXT("WSASend failed with socket error %d."), dwErrorCode);
		closesocket(props->socket);
		return;
	}
	sent += dwNumberOfBytesTransfered;
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
BOOL LoadFile(LPWSABUF wsaBuf, const TCHAR *szFileName, char **buf, LPDWORD lpdwFileSize, LPTransferProps props)
{
	DWORD dwFileSize;
	HANDLE hFile;
	hFile = CreateFile(props->szHostName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("Couldn't Open File"),
			TEXT("Could not open file %s. Please check the spelling or select a different file."), props->szFileName);
		return FALSE;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	*buf = (CHAR *)malloc(dwFileSize);
	ReadFile(hFile, buf, dwFileSize, NULL, NULL);
	wsaBuf->len = dwFileSize;
	*lpdwFileSize = dwFileSize;
}