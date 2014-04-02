#ifndef ASSN2_UTILS
#define ASSN2_UTILS

#include "WinStorage.h"
#include <Windows.h>
#include <tchar.h>
#include <cstdio>
#include "gui/resource.h"
#include "TransferProps.h"

#define TIMESTAMP_SIZE 27

// Convert between TCHAR and char
#ifdef UNICODE
#define TCHAR_2_CHAR(dest, src, bufsize) \
	wcstombs_s(NULL, dest, bufsize, src, bufsize)

#define CHAR_2_TCHAR(dest, src, bufsize) \
	mbstowcs_s(NULL, dest, src, bufsize)
#else
#define TCHAR_2_CHAR(dest, src, size) \
	strcpy_s(dest, src, bufsize)

#define CHAR_2_TCHAR(dest, src, size) \
	strcpy_s(dest, src, bufsize)
#endif

int CDECL MessageBoxPrintf(DWORD dwType, TCHAR * szCaption, TCHAR * szFormat, ...);
VOID LogTransferInfo(const char *filename, LPTransferProps props, DWORD dwSentOrRecvd, HWND hwnd);
VOID CreateTimestamp(char *buf, SYSTEMTIME *time);
VOID LogError(const TCHAR *functionName, const TCHAR *msg);

#endif