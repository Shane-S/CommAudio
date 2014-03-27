/*----------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Main.cpp
--
-- PROGRAM: Assn2
--
-- FUNCTIONS:
-- LPTransferProps CreateTransferProps();
-- int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpszCmdArgs, int iCmdShow);
--
-- DATE: February 1st, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- NOTES: This file contains functions that set up the window and transfer properties for use. It is also the entry point
--		  to the program.
-------------------------------------------------------------------------------------------------------------------------*/

#include "Main.h"
#include "WndProc.h"

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
-- January 30th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpszCmdArgs, int iCmdShow)
--								HINSTANCE hPrevInstance:	Handle to the a previous copy of the program (not used here).
--								HINSTANCE hInstance:		Handle to the current instance of the program.
--								LPSTR lpszCmdArgs:			Pointer to the command arguments (if any).
--								int iCmdShow:				Determines how the window will be shown.
--
-- RETURNS: void
--
-- NOTES:
-- This is the entry point to the program. It sets up the window, creates a TransferProps structure, and sets the default
-- host mode before displaying the GUI.
---------------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpszCmdArgs, int iCmdShow)
{
	HWND			hwnd;
	MSG				msg;
	WNDCLASS		wndclass;
	WSADATA			wsaData;
	LPTransferProps props = CreateTransferProps();

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(DWORD) + sizeof(LPTransferProps);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = CLASS_NAME;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Fatal error; extiting the program"), TEXT("Error"), MB_ICONERROR);
		return 0;
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if ((props = CreateTransferProps()) == NULL)
	{
		MessageBox(NULL, TEXT("Unable to allocate memory for transfer properties."), TEXT("Error"), MB_ICONERROR);
		return -1;
	}

	hwnd = CreateWindow(CLASS_NAME, TEXT("Test Program"), WS_OVERLAPPEDWINDOW,
		0, 0, 600, 600, NULL, NULL, hInstance, NULL);

	SetWindowLongPtr(hwnd, GWLP_TRANSFERPROPS, (LONG)props);
	SetWindowLongPtr(hwnd, GWLP_HOSTMODE, ID_HOSTTYPE_CLIENT);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WSACleanup();
	return msg.wParam;
}
