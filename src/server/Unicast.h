#ifndef UNICAST_H
#define UNICAST_H

#include "ServerTransfer.h"
#include "ServerInfo.h"


// Completion routines for various client requests
VOID CALLBACK UnicastGeneralRecv(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered,LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK UnicastFileSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK SearchResultsSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK VoiceDataRecv(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);
VOID CALLBACK VoiceDataSend(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);

#endif
