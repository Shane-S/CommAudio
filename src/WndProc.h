#ifndef WND_PROC_H
#define WND_PROC_H

#include "gui/TransferDlgProc.h"
#include <Windows.h>
#include "gui/resource.h"
#include "client/ClientTransfer.h"
#include "server/ServerTransfer.h"

LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
#endif