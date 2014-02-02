#ifndef TRANSFER_H
#define TRANSFER_H

#include <WinSock2.h>
#include <Windows.h>
#include <cstdio>
#include <cstring>
#include "WinStorage.h"
#include "Utils.h"

BOOL ClientInitSocket(LPTransferProps);
DWORD WINAPI ClientSendData(VOID * params);

#endif