#ifndef MULTICAST_H
#define MULTICAST_H
#include "ServerTransfer.h"
#include "ServerInfo.h"

#define MULTICAST_ADDR "234.5.6.7"
#define MULTICAST_PORT 7001

VOID CALLBACK MulticastSendComplete(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped, DWORD dwFlags);

#endif
