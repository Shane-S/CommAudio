#include "WndProc.h"

LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			return 0;
		case ID_TRANSFER_BEGINTRANSFER:
		{
			DWORD res;
			if ((res = ClientInitSocket((LPTransferProps)GetWindowLongPtr(hwnd, GWLP_TRANSFERPROPS))) == 0)
				break;
			CreateThread(NULL, 0, ClientSendData, (VOID *)hwnd, 0, NULL);
			break;
		}
		case ID_TRANSFER_PROPERTIES:
			DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, TransferDlgProc);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
