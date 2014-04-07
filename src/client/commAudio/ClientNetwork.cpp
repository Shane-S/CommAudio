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
*                  
------------------------------------------------------------------------------------------------------------------------*/
#include "ClientNetwork.h"

void ClientNetwork::initWinsock()
{
    WSAStartup(MAKEWORD(2,2), &stWSAData);
}

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
int ClientNetwork::connectToTCPServer()
{
    struct sockaddr_in server;
    struct hostent *hp;

    if((serverTCPSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        return -1; //error creating socket
    }

    //Initialize and setup address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(portTCP);
    
    if((hp = gethostbyname(serverIP.c_str())) == NULL)
    {
        return -2; //couldn't find server address
    }

    //Copy server addr
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    WSAAsyncSelect(serverTCPSocket, hwnd, WM_SOCKET, FD_READ | FD_CLOSE); 
    if(connect(serverTCPSocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        return -3; //couldn't connect to server
    }

    return 0; //successfully connected to the server
}

int ClientNetwork::initUDPClient()
{
    struct sockaddr_in server, client;
    struct hostent *hp;

    if((serverUDPSocket = WSASocket(PF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        return -1; //error creating socket
    }

    //Initialize and setup server architecture structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(portUDP);

    if((hp = gethostbyname(serverIP.c_str())) == NULL)
    {
        return -2; //couldn't find server address
    }

    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    memset((char *)&client, 0, sizeof(struct sockaddr_in));
    client.sin_family = AF_INET;
    client.sin_port = htons(0); //bind to any available port
    client.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverUDPSocket, (struct sockaddr *)&client, sizeof(client)) < 0)
    {
        return -3; //couldn't bind name to socket
    }

    serverUDPAddr = server;

    return 0; //UDP client successfully initialized
}

//use when sending audio data to server - can use the same code on the server
//protocol = 0 = TCP;; protocol = 1 = UDP
void ClientNetwork::sendAudioData(void *data, bool isTCP, bool isFile)
{
    char streamDataBuffer[2048];
    HSTREAM streamBuffer;
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF buffer;

    //case string to void*:: void *vp = static_cast<void*>(string);
    std::string *fp = static_cast<std::string*>(data);
    std::string filename = *fp;

    WSAOVERLAPPED ov;
    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    
    if(isFile) //not streaming from memory
    {
        streamBuffer = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_STREAM_DECODE);
    }
    else //streaming from memory
    {
        int length = sizeof(data);
        streamBuffer = BASS_StreamCreateFile(TRUE, (void *)data, 0, sizeof(data), 0);
    }

    while(BASS_ChannelIsActive(streamBuffer))
    {
        DWORD readLength = BASS_ChannelGetData(streamBuffer, streamDataBuffer, 2048);

        buffer.len = readLength;
        buffer.buf = (CHAR*)streamDataBuffer;

        if(!isTCP)
        {
            //UDP
            WSASendTo(serverUDPSocket, &buffer, 1, &SendBytes, 0, (const sockaddr *)&serverUDPAddr, sizeof(serverUDPAddr), &ov, 0);
        }
        else
        {
            //TCP
            WSASend(serverTCPSocket, &buffer, 1, &SendBytes, 0, &ov, NULL);
        }

        ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    }
}

//use when receving audio data from server - can use same code on the server
void ClientNetwork::getAudioData(bool isTCP)
{
    
}

//closes a socket
void ClientNetwork::closeSocket(SOCKET socket)
{
    closesocket(socket);
}

//Wrapper function for terminating the use of Winsock2 DLL
void ClientNetwork::terminateWinSock()
{
    WSACleanup();
}