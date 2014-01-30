/*
 * To do list:
 *
 * TODO: Change the SetDlgDefaults function to actually populate the fields based on LPTransferProps.
 */

/*----------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: HostIPDlg.cpp
--
-- PROGRAM: Assn1
--
-- FUNCTIONS:
-- INT_PTR CALLBACK HostIPDlgProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
-- VOID HostIPSetSelect(const DWORD dwSelected, const HWND hwndDlg)
--
-- DATE: January 14th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- NOTES:	This file contains functions to manage a dialog box with which the user can determine the IP address for a
--			a given host name, or the host name for a given port IP address.
-------------------------------------------------------------------------------------------------------------------------*/
#include "TransferDlgProc.h"

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: HostIPDlgProc
-- January 14th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: INT_PTR CALLBACK HostIPDlgProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
--				HWND hwndDlg:	Handle to the dialog window.
--				UINT uMsg:		The message being passed to this dialog box.
--				WPARAM wParam:	A general-purpose parameter that may contain information relevant to the message.
--				LPARAM lParam:	A general-purpose parameter that may contain information relevant to the message.
--
-- RETURNS: The button selected by the user; IDOK if they click "Lookup", and IDCANCEL if they click "Cancel".
--
-- NOTES:
-- Handles messages related to its controls (users clicking on radio buttons, Resolve, Cancel, etc.) and looks up the
-- IP address corresponding to a host name (or vice versa) if the user chooses to do this. The result is stored in a string
-- to be written in the main listbox.
---------------------------------------------------------------------------------------------------------------------------*/
INT_PTR CALLBACK TransferDlgProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetDlgDefaults(hwndDlg, (LPTransferProps)GetWindowLongPtr(GetParent(hwndDlg), GWLP_TRANSFERPROPS));
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (FillTransferProps(hwndDlg))
				EndDialog(hwndDlg, LOWORD(wParam));
			return TRUE;
		case IDCANCEL:
			EndDialog(hwndDlg, LOWORD(wParam));
			return TRUE;
		case ID_BUTTON_BROWSE:
			OpenFileDlg(hwndDlg);
			return TRUE;
		}
	}
	return FALSE;
}

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SetDlgDefaults
-- January 14th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: VOID SetDlgDefaults(HWND hwndDlg, LPTransferProps props)
--									HWND hwndDlg:		Handle to the dialog window.
--									LPTransferProps:	Pointer to the transfer properties structure.
--
-- RETURNS: void
--
-- NOTES:
-- Initialises the dialog box's menu selections. It also checks the props structure and sets menu items if they are
-- different from the defaults.
---------------------------------------------------------------------------------------------------------------------------*/
VOID SetDlgDefaults(HWND hwndDlg, LPTransferProps props)
{
	HWND hDropDown	= GetDlgItem(hwndDlg, ID_DROPDOWN_SIZE);
	HWND hwndTCPUDP	= GetDlgItem(hwndDlg, ID_RADIO_TCP);
	HWND hwndSend	= GetDlgItem(hwndDlg, ID_RADIO_SEND10);

	SendMessage(hDropDown, CB_ADDSTRING, 0, (LPARAM)TEXT("Use file size"));
	SendMessage(hDropDown, CB_ADDSTRING, 0, (LPARAM)TEXT("1024 (1KB)"));
	SendMessage(hDropDown, CB_ADDSTRING, 0, (LPARAM)TEXT("4096 (4KB)"));
	SendMessage(hDropDown, CB_ADDSTRING, 0, (LPARAM)TEXT("20480 (20KB)"));
	SendMessage(hDropDown, CB_ADDSTRING, 0, (LPARAM)TEXT("61440 (60KB)"));
	SendMessage(hDropDown, CB_SETCURSEL, 0, 0);

	SendMessage(hwndTCPUDP, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
	SendMessage(hwndSend, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
}

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: HostIPDlgProc
-- January 14th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: INT_PTR CALLBACK HostIPDlgProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
--				HWND hwndDlg:	Handle to the dialog window.
--				UINT uMsg:		The message being passed to this dialog box.
--				WPARAM wParam:	A general-purpose parameter that may contain information relevant to the message.
--				LPARAM lParam:	A general-purpose parameter that may contain information relevant to the message.
--
-- RETURNS: The button selected by the user; IDOK if they click "Lookup", and IDCANCEL if they click "Cancel".
--
-- NOTES:
-- Handles messages related to its controls (users clicking on radio buttons, Resolve, Cancel, etc.) and looks up the
-- IP address corresponding to a host name (or vice versa) if the user chooses to do this. The result is stored in a string
-- to be written in the main listbox.
---------------------------------------------------------------------------------------------------------------------------*/
BOOL FillTransferProps(HWND hwndDlg)
{
	TCHAR			buf[FILENAME_SIZE];
	LPTransferProps props			= (LPTransferProps)GetWindowLongPtr(GetParent(hwndDlg), GWLP_TRANSFERPROPS);
	HWND			hwndSendNum		= GetDlgItem(hwndDlg, ID_RADIO_SEND10);
	HWND			hwndProtocol	= GetDlgItem(hwndDlg, ID_RADIO_TCP);
	HWND			hwndDropDown	= GetDlgItem(hwndDlg, ID_DROPDOWN_SIZE);
	DWORD			dwDropDownSel;
	DWORD			dwPacketSize;

	props->nNumToSend = (SendMessage(hwndSendNum, BM_GETCHECK, 0, 0) == BST_CHECKED) ? 10 : 100;
	props->nSockType  = (SendMessage(hwndProtocol, BM_GETCHECK, 0, 0) == BST_CHECKED) ? SOCK_STREAM : SOCK_DGRAM;

	if(!GetDlgAddrInfo(hwndDlg, props))
		return FALSE;

	dwDropDownSel = SendMessage(hwndDropDown, CB_GETCURSEL, 0, 0);
	GetDlgItemText(hwndDlg, ID_TEXTBOX_FILE, buf, BUFSIZE);
	
	//Get the transfer details: packet size, what file (if any) to use, number of packets to send
	if (dwDropDownSel == DROPDOWN_USEFILESIZE)
	{
		if(buf[0] == 0)
		{
			MessageBox(NULL, TEXT("Please specify a file to send."), TEXT("No file"), MB_ICONERROR);
			return FALSE;
		}
		_tcscpy_s(props->szFileName, buf);
		props->nPacketSize = 0; // Use the file size
	}
	else
	{
		ComboBox_GetText(hwndDropDown, buf, FILENAME_SIZE);
		if (_stscanf_s(buf, TEXT("%d"), &dwPacketSize) == 0)
		{
			MessageBox(NULL, TEXT("Please enter a number for packet size."), TEXT("Invalid byte number"), MB_ICONERROR);
			return FALSE;
		}
		props->nPacketSize = dwPacketSize;
	}

	return TRUE;
}

/*-------------------------------------------------------------------------------------------------------------------------
-- FUNCTION: HostIPDlgProc
-- January 14th, 2014
--
-- DESIGNER: Shane Spoor
--
-- PROGRAMMER: Shane Spoor
--
-- INTERFACE: GetDlgAddrInfo(HWND hwndDlg, LPTransferProps props)
--							HWND hwndDlg:			Handle to the dialog box window.
--							LPTransferProps props:	Pointer to a transfer properties struct.
--
-- RETURNS: False if one of the conversions fails, true otherwise.
--
-- NOTES:
-- Retrieves the port number and IP address/host name of entered by the user and places them in the sock_inaddr struct if
-- there are no errors.
---------------------------------------------------------------------------------------------------------------------------*/
BOOL GetDlgAddrInfo(HWND hwndDlg, LPTransferProps props)
{
	TCHAR	buf[HOSTNAME_SIZE];
	USHORT	usPortNum;
	DWORD	dwBinaryAddr;
	char	hostip_string[HOSTNAME_SIZE];

	// Get the information for storage in our in_addr
	GetDlgItemText(hwndDlg, ID_TEXTBOX_PORT, buf, BUFSIZE);
	if(_tscanf_s(buf, "%h", &usPortNum) == 0)
	{
		MessageBox(NULL, TEXT("The port must be a number."), TEXT("Non-Numeric Port"), MB_ICONERROR);
		return FALSE;
	}
	props->paddr_in->sin_port = htons(usPortNum);

	GetDlgItemText(hwndDlg, ID_TEXTBOX_HOSTIP, buf, BUFSIZE);
	if(buf[0] == 0)
	{
		MessageBox(NULL, TEXT("Please enter a host name or IP address."), TEXT("No Host/IP"), MB_ICONERROR);
		return FALSE;
	}

	// IP address (maybe)
	if(isdigit(buf[0]))
	{
		TCHAR_2_CHAR(hostip_string, buf, HOSTNAME_SIZE);
		if((dwBinaryAddr = inet_addr(hostip_string)) == INADDR_NONE)
		{
			MessageBox(NULL, TEXT("IP address must be in the form xxx.xxx.xxx.xxx."), TEXT("Invalid IP"), MB_ICONERROR);
			return FALSE;
		}
		props->paddr_in->sin_addr.s_addr = dwBinaryAddr;
		props->szHostName[0] = 0; // Don't check the host name when sending later 
	}
	else
	{
		props->paddr_in->sin_addr.s_addr = 0; // Get rid of current address

		// We'll check at connection time whether this is a real host; store it for now
		_tcscpy_s(props->szHostName, buf);
	}
	return TRUE;
}

VOID OpenFileDlg(HWND hwndDlg)
{
	OPENFILENAME	ofn;
	TCHAR			szFileName[FILENAME_SIZE] = {0};
	HWND			hwndFile = GetDlgItem(hwndDlg, ID_TEXTBOX_FILE);

	ofn.lStructSize			= sizeof(OPENFILENAME);
	ofn.hwndOwner			= hwndDlg;
	ofn.hInstance			= NULL;
	ofn.Flags				= OFN_EXPLORER | OFN_FORCESHOWHIDDEN | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;
	ofn.lpstrFilter			= NULL;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 0;
	ofn.nFilterIndex		= 0;
	ofn.lpstrFile			= szFileName;
	ofn.nMaxFile			= BUFSIZE;
	ofn.lpstrFileTitle		= NULL;
	ofn.nMaxFileTitle		= NULL;
	ofn.lpstrInitialDir		= NULL;
	ofn.lpstrTitle			= TEXT("Choose a File to Send");
	ofn.lpstrDefExt			= NULL;
	ofn.lCustData			= 0;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;
	ofn.FlagsEx				= 0;

	GetOpenFileName(&ofn);
	SetWindowText(hwndFile, szFileName);
}
