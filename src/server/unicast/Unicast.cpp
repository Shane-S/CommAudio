#include "Unicast.h"
/**
 * Contains completion routines and helper functions for the Unicast portion of the server.
 *
 * @file Unicast.cpp
 *
 * @todo Add error logging mechanism
 * @todo Integrate with backend (database)
 */

/**
 * Reads data into a buffer and sends it to the client.
 *
 * If the transfer was cancelled (the client stopped the stream or the file finished
 * file finished sending), then the function simply returns. On send error, logs the error
 * and returns.
 *
 * @param dwErrorCode                The error code (if any) from an error that occurred while sending.
 * @param dwNumberOfBytesTransferred The number of bytes transferred while sending.
 * @param lpOverlapped               Pointer to the overlapped struct, which is in fact a pointer to a
 *                                   TransferProps struct.
 * @param dwFlags                    Flags specified while sending. Not used here.
 *
 * @designer Shane Spoor
 * @author   Shane Spoor
 */
VOID CALLBACK ServerUniSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	DWORD			bytesRead;
	if (dwErrorCode) //|| dwNumberOfBytesTransfered != props->dataBuffer.len) // error or didn't send everything
	{
		LogError(TEXT("ServerUniSendCompletion"), dwErrorCode);
		props->transferCancelled = TRUE;
		return;
	}
	else if (props->transferCancelled)
	{
		CloseHandle(props->audioFile);
		return;
	}

	ReadFile(props->audioFile, props->dataBuffer.buf, BUFSIZE, &bytesRead, NULL);
	if (!bytesRead) // No more data to read; finished sending
	{
		CloseHandle(props->audioFile);
		props->transferCancelled = TRUE;
		return;
	}
	props->dataBuffer.len = bytesRead;
	WSASend(props->socket, &props->dataBuffer, 1, NULL, 0, &props->wsaOverlapped, ServerUniSendCompletion);
}

/**
 *
 *
 *
 *
 */
VOID CALLBACK ServerUniRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	if (dwErrorCode)
	{
		LogError(TEXT("ServerUniRecvCompltion"), dwErrorCode);
		closesocket(props->socket);
		CloseHandle(props->audioFile);
		props->transferCancelled = TRUE;
		return;
	}

	if (!dwNumberOfBytesTransfered) // Connection closed/reset
	{
		DestroyTransferProps(props);
		return;
	}
}
