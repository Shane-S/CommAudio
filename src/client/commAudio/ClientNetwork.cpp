/*--------------------------------------------------------------------------------------------------------------------
* SOURCE FILE:     ClientNetwork.cpp - Contains all the network related functions for the client.
*
* PROGRAM:         commAudio
*
* FUNCTIONS:       list_of_functions
*
* DATE:            March 31, 2014
*
* REVISIONS:       (Date and Description)
*                  revision_date     revision_description
*
* DESIGNER:        Abhishek Bhardwaj
*
* PROGRAMMER:      Abhishek Bhardwaj
*
* NOTES:           This file contains all functions to handle client side networking with the server.
*                  The functions in this file assume that `WSAStartup()` call has already been made.
*
------------------------------------------------------------------------------------------------------------------------*/
#include "ClientNetwork.h"

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    connectToServer()
*
* DATE:        March 31, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* INTERFACE:   int ClientNetwork::connectToServer()
*                      
* RETURNS:     int
*                       0       connection to server, successful
*                       -1      error creating socket
*                       -2      unknown server address
*                       -3      couldn't connect to the server
*
* NOTES:       Initializes a socket and then sets a TCP connection with the server using OVERLAPPED I/O.
--------------------------------------------------------------------------------------------------------------------------*/
int ClientNetwork::connectToServer()
{
    struct sockaddr_in server;
    struct hostent *hp;

    if((serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        return -1; //error creating socket
    }

    //Initialize and setup address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(portNo);
    
    if((hp = gethostbyname(serverIP.c_str())) == NULL)
    {
        return -2; //couldn't find server address
    }

    //Copy server addr
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    if(connect(serverSocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        return -3; //couldn't connect to server
    }

    return 0; //successfully connected to the server
}

