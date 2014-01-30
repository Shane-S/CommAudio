#ifndef ASSN2_UTILS
#define ASSN2_UTILS

#include <Windows.h>
#include <tchar.h>

// Convert between TCHAR and char
#ifdef UNICODE
#define TCHAR_2_CHAR(dest, src, bufsize) \
	wcstombs_s(NULL, dest, bufsize, src, bufsize)
#else
#define TCHAR_TO_CHAR(dest, src, size) \
	strcpy_s(dest, src, bufsize)
#endif

int CDECL MessageBoxPrintf(DWORD dwType, TCHAR * szCaption, TCHAR * szFormat, ...);

#endif