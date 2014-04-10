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
int main(int argc, char *argv[])
{
	WSADATA	wsaData;
	LPTransferProps props = CreateTransferProps();
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ServerInitSocket(props);
	Serve(props);

	WSACleanup();
	return 0;
}
