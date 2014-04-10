/*--------------------------------------------------------------------------------------------------------------------
* SOURCE FILE:     ClientNetwork.cpp - Contains all the network related functions for the client.
*
* PROGRAM:         commAudio
*
* FUNCTIONS:       
*                   - void initWinsock(); 
*                   - void terminateWinSock();
*                   - int connectToTCPServer();
*                   - int initUDPClient();
*                   - void sendAudioData(void *data, bool isTCP, bool isFile);
*                   - void getAudioData(bool isTCP);
*                   - SOCKET getTCPSocket() {return serverTCPSocket;}
*                   - void setHWND(HWND hwnd_) {hwnd = hwnd_;}
*                   - void closeSocket(SOCKET socket);
*
* DATE:            March 31, 2014
*
* REVISIONS:       (Date and Description)
*                  April 4, 2014     Added a bunch of extra functions.
*
* DESIGNER:        Abhishek Bhardwaj
*
* PROGRAMMER:      Abhishek Bhardwaj
*
* NOTES:           This file contains all functions to handle client side networking with the server.
*                  
------------------------------------------------------------------------------------------------------------------------*/
#include "ClientNetwork.h"

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    initWinsock()
*
* DATE:        April 4th, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* INTERFACE:   void ClientNetwork::initWinsock()
*
* RETURNS:     void
*                    
* NOTES:       A wrapper function that initiates the use of the Winsock DLL.
--------------------------------------------------------------------------------------------------------------------------*/
void ClientNetwork::initWinsock()
{
    WSAStartup(MAKEWORD(2,2), &stWSAData);
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    connectToTCPServer()
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
* INTERFACE:   int ClientNetwork::connectToTCPServer()
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
    server.sin_port = htons(connectionSettings.getPortTCP());
    
    if((hp = gethostbyname(connectionSettings.getIpAddress().c_str())) == NULL)
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

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    initUDPClient()
*
* DATE:        April 4, 2014
*
* REVISIONS:   (Date and Description)
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* INTERFACE:   int ClientNetwork::initUDPClient()
*                      
* RETURNS:     int
*                       0       connection to server, successful
*                       -1      error creating socket
*                       -2      unknown server address
*                       -3      couldn't bind name to socket
*
* NOTES:       Initializes a UDP client socket and sets it up with required parameters.
--------------------------------------------------------------------------------------------------------------------------*/
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
    server.sin_port = htons(connectionSettings.getPortUDP());

    if((hp = gethostbyname(connectionSettings.getIpAddress().c_str())) == NULL)
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

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    sendAudioData(void *data, bool isTCP, bool isFile)
*
* DATE:        April 4, 2014
*
* REVISIONS:   (Date and Description)
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* INTERFACE:   int ClientNetwork::sendAudioData(void *data, bool isTCP, bool isFile)
*                           void *data  - data to be sent to the server [could be a filename or pointer to a memory location]
*                           bool isTCP  - switch to specify what protocol to use to send the data [true for TCP, false for UDP]
*                           bool isFile - switch to specify if the *data is an actual file on disk or a pointer to a memory location.
* RETURNS:     void
*
* NOTES:       Sends audio data to the server (capable to do it with both TCP or UDP). Audio data could be an actual file
*              or an actual memory location.
--------------------------------------------------------------------------------------------------------------------------*/
void ClientNetwork::sendAudioData(void *data, bool isTCP, bool isFile)
{
    char streamDataBuffer[2048];
    HSTREAM streamBuffer;
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF buffer;

    WSAOVERLAPPED ov;
    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    
    if(isFile) //not streaming from memory
    {
        //cast string to void*:: void *vp = static_cast<void*>(string);
        std::string *fp = static_cast<std::string*>(data);
        std::string filename = *fp;
        streamBuffer = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_STREAM_DECODE);
    }
    else //streaming from memory
    {
        int length = sizeof(data);
        streamBuffer = BASS_StreamCreateFile(TRUE, (void *)data, 0, sizeof(data), 0);
        //streamBuffer = BASS_StreamCreateFile(TRUE, NULL, 0, sizeof(data), 0);
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

void ClientNetwork::sendMicData(HRECORD micHandle)
{
    char streamDataBuffer[2048];
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF buffer;

    WSAOVERLAPPED ov;
    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    
    while(BASS_ChannelIsActive(micHandle))
    {
        DWORD readLength = BASS_ChannelGetData(micHandle, streamDataBuffer, 2048);

        buffer.len = readLength;
        buffer.buf = streamDataBuffer;

        //UDP
        WSASendTo(serverUDPSocket, &buffer, 1, &SendBytes, 0, (const sockaddr *)&serverUDPAddr, sizeof(serverUDPAddr), &ov, 0);

        ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
    }
}

void ClientNetwork::sendPing()
{
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF buffer;
    buffer.len = sizeof(char);
    buffer.buf = (char *) "This is a fucking test";
    WSAOVERLAPPED ov;
    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
  
    //TCP
    WSASend(serverTCPSocket, &buffer, 1, &SendBytes, 0, &ov, NULL);

    ZeroMemory(&ov, sizeof(WSAOVERLAPPED));
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