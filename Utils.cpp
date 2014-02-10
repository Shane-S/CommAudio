#include "Utils.h"

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

int CDECL DrawTextPrintf(HWND hwnd, TCHAR * szFormat, ...)
{
	HDC hdc = GetDC(hwnd);
	TCHAR szBuffer[1024];
	va_list pArgList;
	DWORD dwRet;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	// The va_start macro (defined in STDARG.H) is usually equivalent to:
	// pArgList = (char *) &szFormat + sizeof (szFormat) ;
	va_start(pArgList, szFormat);
	// The last argument to wvsprintf points to the arguments
	_vsntprintf_s(szBuffer, sizeof (szBuffer) / sizeof (TCHAR),
		szFormat, pArgList);
	// The va_end macro just zeroes out pArgList for no good reason
	va_end(pArgList);

	dwRet = DrawText(hdc, szBuffer, _tcslen(szBuffer), &clientRect, DT_CALCRECT);
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
	ReleaseDC(hwnd, hdc);

	return dwRet;
}

VOID LogTransferInfo(const char *filename, LPTransferProps props, DWORD dwSentOrRecvd, DWORD dwHostMode)
{
	FILE *file;
	FILETIME ftStartTime, ftEndTime;
	CHAR startTimestamp[TIMESTAMP_SIZE] = { 0 }, endTimestamp[TIMESTAMP_SIZE] = { 0 };
	ULARGE_INTEGER ulStartTime, ulEndTime, ulTransferTime;

	// Jump through the ludicrous amount of hoops to get millisecond resolution on Windows
	SystemTimeToFileTime(&props->startTime, &ftStartTime);
	SystemTimeToFileTime(&props->endTime, &ftEndTime);

	ulStartTime.QuadPart = ftStartTime.dwHighDateTime;
	ulStartTime.QuadPart <<= 32;
	ulStartTime.QuadPart += ftStartTime.dwLowDateTime;

	ulEndTime.QuadPart = ftEndTime.dwHighDateTime;
	ulEndTime.QuadPart <<= 32;
	ulEndTime.QuadPart += ftEndTime.dwLowDateTime;

	ulTransferTime.QuadPart = ulEndTime.QuadPart - ulStartTime.QuadPart;

	CreateTimestamp(startTimestamp, &props->startTime);
	CreateTimestamp(endTimestamp, &props->endTime);

	fopen_s(&file, filename, "a");
	if (file == NULL)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("Couldn't Open File"), TEXT("Couldn't open log file %s"), filename);
		return;
	}

	if (props->nSockType == SOCK_STREAM)
		dwSentOrRecvd /= props->nPacketSize; // Number of TCP 'packets' sent/received (Nagle may have given different ones)

	// The division by 10 000 is necessary because Windows gives the times in 100ns intervals. Why would you do that. Seriously.
	fprintf(file, "Start timestamp: %s\r\nEnd timestamp: %s\r\nTransfer time: %dms\r\n", startTimestamp, endTimestamp,
		ulTransferTime.QuadPart / 10000);
	fprintf(file, "Packet size: %d bytes\r\n", props->nPacketSize);
	
	if(dwHostMode == ID_HOSTTYPE_SERVER)
		fprintf(file, "Packets received : %d\r\nPackets expected : %d\r\n", dwSentOrRecvd, props->nNumToSend);
	else
		fprintf(file, "Packets sent: %d\r\n", dwSentOrRecvd);

	fprintf(file, "Protocol: %s\r\n\r\n", (props->nSockType == SOCK_DGRAM) ? "UDP" : "TCP");
	fclose(file);
}


VOID CreateTimestamp(char *buf, SYSTEMTIME *time)
{
	sprintf_s(buf, TIMESTAMP_SIZE, "%d-%02d-%02dT%02d:%02d:%02d:%03dTZD", time->wYear, time->wMonth, time->wDay, time->wHour, 
		time->wMinute, time->wSecond, time->wMilliseconds);
}
