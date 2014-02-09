#include "WndProc.h"

extern LPTransferProps props;

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
			DWORD dwHostMode = (DWORD)GetWindowLongPtr(hwnd, GWLP_HOSTMODE);
			LPTransferProps props = (LPTransferProps)GetWindowLongPtr(hwnd, GWLP_TRANSFERPROPS);

			if (dwHostMode == ID_HOSTTYPE_CLIENT)
			{
				if (!ClientInitSocket(props))
					break;
				CreateThread(NULL, 0, ClientSendData, (VOID *)hwnd, 0, NULL);
			}
			else
			{
				if (!ServerInitSocket(props))
					break;
				CreateThread(NULL, 0, Serve, (VOID *)hwnd, 0, NULL);
			}
			break;
		}
		case ID_TRANSFER_PROPERTIES:
			DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, TransferDlgProc);
			break;

		case ID_HOSTTYPE_CLIENT:
		case ID_HOSTTYPE_SERVER:
		{
			HMENU hMenu = GetMenu(hwnd);
			DWORD dwCurChecked = GetWindowLongPtr(hwnd, GWLP_HOSTMODE);
			if (dwCurChecked == LOWORD(wParam))
				break;
			CheckMenuItem(hMenu, LOWORD(wParam), MF_CHECKED);
			CheckMenuItem(hMenu, dwCurChecked, MF_UNCHECKED);
			SetWindowLongPtr(hwnd, GWLP_HOSTMODE, (LONG)LOWORD(wParam));
			break;
		}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
