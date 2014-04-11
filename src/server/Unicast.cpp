/**
 * Contains completion routines and helper functions for the Unicast portion of the server.
 *
 * @file Unicast.cpp
 *
 * @todo Add error logging mechanism
 * @todo Integrate with backend (database)
 */

#include "Unicast.h"
extern std::unique_ptr<AudioLibrary> lib;
extern HSTREAM streamBuffer;

/**
* Receives and delegates file streaming commands from clients.
*
* Handles the initial handshake (after accept) and calls the appropriate handlers for other packet
* types.
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
VOID CALLBACK UnicastGeneralRecv(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	LPTransferProps props = (LPTransferProps)lpOverlapped;
	if (dwErrorCode)
	{
		TCHAR err_buf[8];
		//LogError(TEXT("ServerUniRecvCompletion"), itoa(dwErrorCode, err_buf, 10));
	}

	if (!dwNumberOfBytesTransfered) // Connection closed/reset
	{
		DestroyTransferProps(props);
		return;
	}
}

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
VOID CALLBACK UnicastFileSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	ClientStruct *client = (ClientStruct *)lpOverlapped;
	Client *actualClient = client->client;
	WSABUF buffer;

	if (dwErrorCode) //|| dwNumberOfBytesTransfered != props->dataBuffer.len) // error or didn't send everything
	{
		return;
	}
	char streamDataBuffer[BUFSIZE];
	DWORD readLength = 0;
	DWORD SendBytes = 0;

	readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, BUFSIZE);
	if (readLength == -1)
	{
		int err = BASS_ErrorGetCode();
		return;
	}
	buffer.len = BUFSIZE;
	buffer.buf = streamDataBuffer;

	WSASend(actualClient->getSock(), &buffer, 1, NULL, 0, &client->fakeOvr, UnicastFileSend);
}

/**
 * Reads the search results from
 *
 *
 *
 */
VOID CALLBACK SearchResultsSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{

}

/**
 *
 *
 *
 *
 */
VOID CALLBACK VoiceDataRecv(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);

/**
 *
 *
 *
 *
 *
 *
 *
 */
VOID CALLBACK VoiceDataSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);
