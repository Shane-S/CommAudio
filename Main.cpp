#include "Main.h"
#include "WndProc.h"

int WINAPI WinMain(HINSTANCE hPrevInstance, HINSTANCE hInstance, LPSTR lpszCmdArgs, int iCmdShow)
{
	HWND			hwnd;
	MSG				msg;
	WNDCLASS		wndclass;
	LPTransferProps props;
	WSADATA			wsaData;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(LPTransferProps) + sizeof(DWORD);
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

LPTransferProps CreateTransferProps()
{
	LPTransferProps props = (LPTransferProps)malloc(sizeof(TransferProps));
	if(props == NULL)
		return NULL;
	
	memset(props->szFileName, 0, FILENAME_SIZE);
	memset(props->szHostName, 0, HOSTNAME_SIZE);

	props->nPacketSize = DEF_PACKETSIZE;

	props->paddr_in = (LPSOCKADDR_IN)malloc(sizeof(SOCKADDR_IN));
	if(props->paddr_in == NULL)
	{
		free(props);
		return NULL;
	}

	memset(props->paddr_in, 0, sizeof(sockaddr_in));

	props->paddr_in->sin_addr.s_addr	= 0;
	props->paddr_in->sin_family			= AF_INET;
	props->paddr_in->sin_port			= htons(DEF_PORTNUM);
	props->socket = 0;

	return props;
}
