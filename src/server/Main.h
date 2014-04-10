#ifndef MAIN_H
#define MAIN_H

#include "ServerTransfer.h"

// Name constants
#define CLASS_NAME	TEXT("Assn2")
#define APP_NAME	TEXT("TCP v UDP")

// Default transfer properties
#define DEF_PACKETSIZE	1024
#define DEF_PORTNUM		7000
#define DEF_NUMTOSEND	10

int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpszCmdArgs, int iCmdShow);

#endif
