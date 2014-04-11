
/**
 * This file contains utility functions for use (mostly) throughout the entire program.
 *
 * @file Utils.cpp
 *
 * @todo Change LogError to actually log the error as well.
 * @todo Add format string printing to LogError
 */

#include "Utils.h"

/**
 * Prints the function name and error to the error log.
 *
 * @param functionName The name of the function where the error occurred.
 * @param msg		   The error message to log with the function name.
 */
VOID LogError(const TCHAR *functionName, const TCHAR *msg)
{
	MessageBoxPrintf(MB_ICONERROR, TEXT("Error"), TEXT("Error in %s: %s"), functionName, msg);
	return;
}

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MessageBoxPrintf
-- Febrary 7th, 2014
--
-- DESIGNER: Charles Petzold
--
-- PROGRAMMER: Charles Petzold
--
-- INTERFACE: MessageBoxPrintf(DWORD dwType, TCHAR *szCaption, TCHAR *szFormat, ...)
--								DWORD dwType:		The type of message box to display.
--								TCHAR *szCaption:	The caption to display at the top of the message box.
--								TCHAR *szFormat:	The format argument for sprintf.
--								...:				Variadic arguments for use with the format.
--
-- RETURNS: An int; this is the message box's return value.
--
-- NOTES:
-- Wraps sprintf and message box functionality for more convenient custom message box messages.  Note that Petzold also
-- wrote the comments wihtin the function.
---------------------------------------------------------------------------------------------------------------------------*/
int CDECL MessageBoxPrintf(DWORD dwType, TCHAR * szCaption, TCHAR * szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	// The va_start macro (defined in STDARG.H) is usually equivalent to:
	// pArgList = (char *) &szFormat + sizeof (szFormat) ;
	va_start(pArgList, szFormat);
	// The last argument to wvsprintf points to the arguments
	_vsntprintf_s(szBuffer, sizeof (szBuffer) / sizeof (TCHAR),
		szFormat, pArgList);
	// The va_end macro just zeroes out pArgList for no good reason
	va_end(pArgList);
	return MessageBox(NULL, szBuffer, szCaption, dwType);
}

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: LogTransferInfo
-- Febrary 9th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: LogTransferInfo(const char *filename, LPTransferProps props, DWORD dwSentOrRecvd, DWORD dwHostMode)
--								char *filename:			The name of the log file.
--								LPTransferProps props:	Pointer to the TransferProps structure containing details about this
--														transfer.
--								DWORD dwSentOrRecvd:	The number of bytes/packets sent or received.
--								DWORD dwHostMode:		The host mode (client or server) in which we're operating.
--
-- RETURNS: void
--
-- NOTES:
-- Logs information about the transfer: start timestamp, end timestamp, transfer time, number of packets
-- sent/received/expected, packet size, and protocol used.
---------------------------------------------------------------------------------------------------------------------------*/
/*VOID LogTransferInfo(const char *filename, LPTransferProps props, DWORD dwSentOrRecvd, HWND hwnd)
{
	//DWORD			dwHostMode = (DWORD)GetWindowLongPtr(hwnd, GWLP_HOSTMODE);
	FILE			*file;
	CHAR			log[512] = { 0 };
	INT				written = 0;
	TCHAR			logw[512];
}*/

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: CreateTimestamp
-- Febrary 9th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: MessageBoxPrintf(char *buf, SYSTEMTIME *time)
--								char *buf:			A buffer which will hold the timestamp string.
--								SYSTEMTIME *time:	The SYSTEMTIME structure holding the timestamp for conversion.
--
-- RETURNS: void
--
-- NOTES:
-- Formats the time in the buffer into a standard timestamp. Surprisingly, Windows doesn't seem to already offer a function
-- for this.
---------------------------------------------------------------------------------------------------------------------------*/
VOID CreateTimestamp(char *buf, SYSTEMTIME *time)
{
	sprintf_s(buf, TIMESTAMP_SIZE, "%d-%02d-%02dT%02d:%02d:%02d:%03dTZD", time->wYear, time->wMonth, time->wDay, time->wHour, 
		time->wMinute, time->wSecond, time->wMilliseconds);
}
