#ifndef MAIN_H
#define MAIN_H

#include <WinSock2.h>
#include <Windows.h>
#include <cstring>
#include "WinStorage.h"

// Name constants
#define CLASS_NAME	TEXT("Assn2")
#define APP_NAME	TEXT("TCP v UDP")

LPTransferProps CreateTransferProps();

#endif