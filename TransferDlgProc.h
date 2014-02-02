#ifndef TRANSFER_DLG_PROC_H
#define TRANSFER_DLG_PROC_H

#include <WinSock2.h>
#include <Windows.h>
#include <cstdio>
#include <tchar.h>
#include "resource.h"
#include "WinStorage.h"
#include "Utils.h"

#define ID_RADIO_TCP		IDC_RADIO1
#define ID_RADIO_UDP		IDC_RADIO2
#define ID_DROPDOWN_SIZE	IDC_COMBO1
#define ID_DROPDOWN_SEND	IDC_COMBO2
#define ID_TEXTBOX_FILE		IDC_EDIT3
#define ID_TEXTBOX_HOSTIP	IDC_EDIT1
#define ID_TEXTBOX_PORT		IDC_EDIT2
#define ID_BUTTON_BROWSE	IDC_BUTTON1

#define DROPDOWN_USEFILESIZE 0

INT_PTR CALLBACK TransferDlgProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID SetDlgDefaults(HWND hwndDlg, LPTransferProps props);
BOOL FillTransferProps(HWND hwndDlg);
BOOL GetDlgAddrInfo(HWND hwndDlg, LPTransferProps);
VOID OpenFileDlg(HWND hwndDlg);

#endif
