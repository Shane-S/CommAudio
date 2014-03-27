#ifndef UNICAST_H
#define UNICAST_H
#include "../ServerTransfer.h"

VOID CALLBACK ServerUniRecvCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
	LPOVERLAPPED lpOverlapped, DWORD dwFlags);
//VOID CALLBACK ServerUniSendCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,
//	LPOVERLAPPED lpOverlapped, DWORD dwFlags);

#endif