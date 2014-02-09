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
