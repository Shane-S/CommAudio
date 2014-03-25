#include "Unicast.h"

/**
 * 
 */
VOID CALLBACK ServerUniSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	if (dwErrorCode)
	{

	}
}

/**
 *
 *
 *
 *
 */
VOID CALLBACK ServerUniRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);