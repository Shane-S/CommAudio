#include "TransferProps.h"


/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: CreateTransferProps
--
-- DATE: Febrary 1st, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: CreateTransferProps()
--
-- RETURNS: A pointer to a TransferProps structure, or NULL if the structure couldn't be created (due to insufficient memory).
--
-- NOTES:
-- This function allocates memory for and initialises a TransferProps structure. It sets the default transfer properties
-- which will be used in case the user decides to begin transferrring without actually choosing options.
---------------------------------------------------------------------------------------------------------------------------*/
LPTransferProps CreateTransferProps()
{
	LPTransferProps props = (LPTransferProps)malloc(sizeof(TransferProps));
	if (props == NULL)
		return NULL;

	props->paddr_in = (LPSOCKADDR_IN)malloc(sizeof(SOCKADDR_IN));
	if (props->paddr_in == NULL)
	{
		free(props);
		return NULL;
	}

	memset(props->paddr_in, 0, sizeof(sockaddr_in));
	props->paddr_in->sin_addr.s_addr = inet_addr("127.0.0.1");
	props->paddr_in->sin_family = AF_INET;
	props->paddr_in->sin_port = htons(PORTNUM);

	props->listenSocket = 0;
	props->nSockType = SOCK_STREAM;

	memset(&props->wsaOverlapped, 0, sizeof(WSAOVERLAPPED));
	props->dataBuffer.buf = (CHAR *)malloc(BUFSIZE);
	if (!props->dataBuffer.buf)
	{
		free(props->paddr_in);
		free(props);
		return NULL;
	}

	props->dataBuffer.len = BUFSIZE;

	props->dwTimeout = COMM_TIMEOUT;
	props->transferCancelled = FALSE;
	return props;
}

VOID DestroyTransferProps(LPTransferProps props)
{
	closesocket(props->listenSocket);
	CloseHandle(props->audioFile);
	free(props->paddr_in);
	free(props->dataBuffer.buf);
	free(props);
}
