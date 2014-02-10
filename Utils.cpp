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
	time_t transferTime = props->endTime - props->startTime;

	fopen_s(&file, filename, "a");
	if (file == NULL)
	{
		MessageBoxPrintf(MB_ICONERROR, TEXT("Couldn't Open File"), TEXT("Couldn't open log file %s"), filename);
		return;
	}

	if (props->nSockType == SOCK_STREAM)
		dwSentOrRecvd /= props->nPacketSize; // Number of TCP 'packets' sent/received; necessary b/c of Nagle

	fprintf(file, "Start timestamp: %llu\r\nEnd timestamp: %llu\r\nTransfer time: %ds\r\n", props->startTime, props->endTime,
		transferTime);
	fprintf(file, "Packet size: %d bytes\r\n", props->nPacketSize);
	
	if(dwHostMode == ID_HOSTTYPE_SERVER)
		fprintf(file, "Packets received : %d\r\nPackets expected : %d\r\n", dwSentOrRecvd, props->nNumToSend);
	else
		fprintf(file, "Packets sent: %d\r\n", dwSentOrRecvd);

	fprintf(file, "Protocol: %s\r\n\r\n", (props->nSockType == SOCK_DGRAM) ? "UDP" : "TCP");
	fclose(file);
}
