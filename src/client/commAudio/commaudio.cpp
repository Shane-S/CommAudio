/*--------------------------------------------------------------------------------------------------------------------
* SOURCE FILE:     commAudio.cpp - Main file for the commAudio program
*
* PROGRAM:         commAudio
*
* FUNCTIONS:            bool nativeEvent(const QByteArray &eventType, void *message, long *result);
*                       void addChatMessageToHistory(QString username, QString message);
*                       void audioPlayback();
*                       void newConnectDialog();
*                       void newAudioUploadDialog();
*                       void playPauseButtonClick();
*                       void sendMessageButtonClick();
*                       void pushToTalkButtonPressed();
*                       void pushToTalkButtonReleased();
*                       void saveToFileCheckboxHandler();
*                       int  multicastSwitch();
*
* DATE:            April 11, 2014
*
* REVISIONS:       (Date and Description)
*                  revision_date     revision_description
*
* DESIGNER:        Abhishek Bhardwaj, Ramzi Chennafi
*
* PROGRAMMER:      Abhishek Bhardwaj
*
* NOTES:           Main functions for the commAudio program
*
------------------------------------------------------------------------------------------------------------------------*/
#include "commaudio.h"
#include "ConnectDialog.h"
#include "UploadAudioDialog.h"
#include <iostream>

//constructor for the commAudio window
commAudio::commAudio(QWidget *parent)
    : QMainWindow(parent)
{
    
    playerState = 0; //start player in paused state
    recording = false;
    saveFile = false;
    connected = false;
    checkHandshake = 0;

    BASS_Init(-1, 44100, 0, 0, NULL);
    BASS_RecordInit(-1);

    clientNetwork.setHWND((HWND)this->winId());
    ui.setupUi(this);
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    saveFile
*
* DATE:        11 April 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Ramzi Chennafi
*
* PROGRAMMER:  ramzi Chennafi
*
* INTERFACE:   function_prototype
*                      char *buffer audio buffer
*                       int bufsize size of the buffer
*                       HSTREAM stream stream pointer
*
* RETURNS:     void
*
* NOTES:       Saves currently playing buffer to a file
--------------------------------------------------------------------------------------------------------------------------*/
void saveFile(const char * buffer, int bufsize, HSTREAM stream)
{
	FILE * fp;
	BASS_CHANNELINFO info;
	WAVEFORMATEX wf;

	if ((fp = fopen("stream.wav", "wb")) == 0)
	{
		std::cout << "Failed to open file" << std::endl;
		return;
	}

	//get and save the channel information
	BASS_ChannelGetInfo(stream, &info);

	wf.wFormatTag = 1;
	wf.nChannels = info.chans;
	wf.wBitsPerSample = (info.flags&BASS_SAMPLE_8BITS ? 8 : 16);
	wf.nBlockAlign = wf.nChannels*wf.wBitsPerSample / 8;
	wf.nSamplesPerSec = info.freq;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec*wf.nBlockAlign;
	//write the header to the file first.
	fwrite("RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20, 1, fp);
	fwrite(&wf, 16, 1, fp);

	fwrite(buffer, sizeof(char), bufsize, fp);

}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    getAudioDataCallback
*
* DATE:        11 April 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* RETURNS:     void
*
* NOTES:       called when audio data is received from the server.
--------------------------------------------------------------------------------------------------------------------------*/
void CALLBACK getAudioDataCallback(IN DWORD dwError, IN DWORD cbTransferred, IN LPWSAOVERLAPPED lpOverlapped, IN DWORD dwFlags)
{
    PAUDIORECEIVESTRUCT audioStruct = (PAUDIORECEIVESTRUCT)lpOverlapped;
    
    if(cbTransferred > 0)
    {
        int err = BASS_StreamPutData(audioStruct->streamH, audioStruct->buffer.buf, cbTransferred);
        err = BASS_ErrorGetCode();
        saveFile(audioStruct->buffer.buf, cbTransferred, audioStruct->streamH);
    }
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    nativeEvent
*
* DATE:        11 Apr, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* RETURNS:     true if we're intercepting the event, false if we're giving it to qt 
*
* NOTES:       Function to interpret windows messages in QT. 
--------------------------------------------------------------------------------------------------------------------------*/
bool commAudio::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* recvMessage = reinterpret_cast<MSG*>(message);

    DWORD bytesReceived;
    DWORD flags;
    static int totalBytesRecv;

    if(recvMessage->message == WM_SOCKET)
    {
        if (WSAGETSELECTERROR(recvMessage->lParam))
        {
            WORD lol = HIWORD(recvMessage->lParam);
        }
        switch(WSAGETSELECTEVENT(recvMessage->lParam))
        {
            case FD_READ:
            {
                flags = 0;

                PAUDIORECEIVESTRUCT audioStruct = (PAUDIORECEIVESTRUCT) malloc(sizeof(PAUDIORECEIVESTRUCT));
                memset(&audioStruct->fakeOverlapped, 0, sizeof(WSAOVERLAPPED));
                audioStruct->streamH = streamHandle;

                if(recvMessage->wParam == clientNetwork.getTCPSocket())
                {
                    audioStruct->buffer.len = STREAM_BUFFER_LENGTH;
                    audioStruct->buffer.buf = (char *) malloc(audioStruct->buffer.len);
                    int err = WSARecv(clientNetwork.getTCPSocket(), &audioStruct->buffer, 1, &bytesReceived, &flags, (LPOVERLAPPED)audioStruct, getAudioDataCallback);
                }
                else if (recvMessage->wParam == clientNetwork.getUDPSocket())
                {
                    audioStruct->buffer.len = AUDIO_BUFFER_LENGTH;
                    audioStruct->buffer.buf = (char *) malloc(audioStruct->buffer.len);
                    int sizeOf = sizeof(clientNetwork.getUDPSockAddr());
                    int err = WSARecvFrom(clientNetwork.getUDPSocket(), &audioStruct->buffer, 1, &bytesReceived, &flags, (sockaddr *)&clientNetwork.getUDPSockAddr(), &sizeOf, (LPOVERLAPPED)audioStruct, getAudioDataCallback);
                    err = WSAGetLastError();
                }
                break;
            }
            case FD_WRITE:
            {                
                if(checkHandshake == 0)
                {
                    connected = true;
                    clientNetwork.sendPing();
                    checkHandshake = 1;
                }
                break;
            }
            case FD_CLOSE:
            {
                connected = false;
                checkHandshake = 0;
                clientNetwork.closeSocket(true);
                clientNetwork.closeSocket(false);

                break;
            }
        }

        return true;
    }

    return false;
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    MyRecordProc
*
* DATE:        11 Apr, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Ramzi Chennafi, Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* RETURNS:     true if succesful
*
* NOTES:       callback for recording from microphone
--------------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK MyRecordProc(HRECORD handle, const void *buffer, DWORD length, void *user)
{
    DWORD SendBytes = 0;
	DWORD BytesTransferred = 0;
	WSABUF WSbuffer;
    PCONNECTIONSTRUCT cStruct = (PCONNECTIONSTRUCT) user;
	int err, size;
	char * buf = (char*)buffer;
	int numChunks = length / AUDIO_BUFFER_LENGTH;

	WSbuffer.len = AUDIO_BUFFER_LENGTH;

	while (numChunks)
	{
		WSbuffer.buf = buf;
        err = WSASendTo(cStruct->UDPSocket, &WSbuffer, 1, &SendBytes, 0, (sockaddr*)&cStruct->UDPSockAddr, sizeof(cStruct->UDPSockAddr), NULL, NULL);
		buf += AUDIO_BUFFER_LENGTH;
		numChunks--;
	}

	if (size = (length % AUDIO_BUFFER_LENGTH))
	{
		WSbuffer.buf = buf;
		WSbuffer.len = size;
		err = WSASendTo(cStruct->UDPSocket, &WSbuffer, 1, &SendBytes, 0, (sockaddr*)&cStruct->UDPSockAddr, sizeof(cStruct->UDPSockAddr), NULL, NULL);
	}

	return TRUE; // continue recording
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    newConnectDialog()
*
* DATE:        11 Apr, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* RETURNS:     void
*
* NOTES:       creates the connection dialog, gets connection params from the user and then initializes BASS
--------------------------------------------------------------------------------------------------------------------------*/
void commAudio::newConnectDialog()
{
    playerState = 0; //start player in paused state

    if(connected == true)
    {
        clientNetwork.closeSocket(true);
        clientNetwork.closeSocket(false);
        connected = false;
    }

    ConnectDialog *connectDialog = new ConnectDialog;
    //connectDialog->show();
    connectDialog->exec();
    
    connectionSettings = connectDialog->connectionSettings;
    clientNetwork.setConnectionSettings(connectionSettings);

    clientNetwork.initWinsock();
    clientNetwork.connectToTCPServer();
    clientNetwork.initUDPClient();

    streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);
    
    PCONNECTIONSTRUCT cStruct = (PCONNECTIONSTRUCT) malloc(sizeof(PCONNECTIONSTRUCT));

    cStruct->UDPSocket = clientNetwork.getUDPSocket();
    cStruct->UDPSockAddr = clientNetwork.getUDPSockAddr();
    micHandle = BASS_RecordStart(44100, 2, BASS_RECORD_PAUSE, MyRecordProc, (void*)cStruct);
}

//makes a new audio upload dialog
void commAudio::newAudioUploadDialog()
{
    UploadAudioDialog *uploadAudioDialog = new UploadAudioDialog;
    uploadAudioDialog->show();
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    Multicast Switch
*
* DATE:        11 Apr, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Ramzi Chennafi
*
* PROGRAMMER:  Ramzi Chennafi, Abhishek Bhardwaj
*
* RETURNS:     true if succesful
*
* NOTES:       does the multicast session.
--------------------------------------------------------------------------------------------------------------------------*/
int commAudio::multicastSwitch()
{
    int nRet;
	BOOL  fFlag;
	SOCKADDR_IN stLclAddr, stSrcAddr;
	struct ip_mreq stMreq;         /* Multicast interface structure */
	SOCKET hSocket;
	char achInBuf[4096];
	WSADATA stWSAData;

	nRet = WSAStartup(0x0202, &stWSAData);
	if (nRet) {
		printf("WSAStartup failed: %d\r\n", nRet);
		exit(1);
	}

	HSTREAM streamHandle = BASS_StreamCreate(44100, 2, 0, STREAMPROC_PUSH, 0);

	/* Get a datagram socket */
	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET) {
		printf("socket() failed, Err: %d\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}

	fFlag = TRUE;
	nRet = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() SO_REUSEADDR failed, Err: %d\n",
			WSAGetLastError());
	}

	/* Name the socket (assign the local port number to receive on) */
	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port = htons(7001);
	nRet = bind(hSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR) {
		printf("bind() port: %d failed, Err: %d\n", 7001,
			WSAGetLastError());
	}

	/* Join the multicast group so we can receive from it */
	stMreq.imr_multiaddr.s_addr = inet_addr("234.5.6.7");
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() IP_ADD_MEMBERSHIP address %s failed, Err: %d\n", "234.5.6.7", WSAGetLastError());
	}

	for (;;) {
		int addr_size = sizeof(struct sockaddr_in);

		nRet = recvfrom(hSocket, achInBuf, 4096, 0, (struct sockaddr*)&stSrcAddr, &addr_size);
		if (nRet < 0) {
			printf("recvfrom() failed, Error: %d\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		int err = BASS_StreamPutData(streamHandle, achInBuf, nRet);
		BASS_ChannelPlay(streamHandle, FALSE);
	} /* end for(;;) */

	/* Leave the multicast group: With IGMP v1 this is a noop, but
	*  with IGMP v2, it may send notification to multicast router.
	*  Even if it's a noop, it's sanitary to cleanup after one's self.
	*/
	stMreq.imr_multiaddr.s_addr = inet_addr("234.5.6.7");
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nRet = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR) {
		printf("setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n", "234.5.6.7", WSAGetLastError());
	}

	closesocket(hSocket);
	WSACleanup();
}

/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    playPauseButtonClick()
*
* DATE:        11 Apr, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* RETURNS:     void
*
* NOTES:       toggle to start or pause playing
--------------------------------------------------------------------------------------------------------------------------*/
void commAudio::playPauseButtonClick()
{
    (playerState == 0) ? playerState = 1 : playerState = 0;
    
    switch(playerState)
    {
        case 0: //currently paused
        {
            //player is paused..person wants to play so we play and then change icons
            QIcon icon("Resources/play.png");
            ui.player_play_pause_toggle_btn->setIcon(icon);

            break;
        }
        case 1: //currently playing
        {
            //player is playing..person wants to pause so we pause and then change icons
            QIcon icon("Resources/pause.png");
            ui.player_play_pause_toggle_btn->setIcon(icon);
            
            std::thread playback(&commAudio::audioPlayback, this);
            playback.detach();

            break;
        }
        default: //shouldn't happen
        {
            //this should never occur. Give a warnning if this happens and put it to paused mode.
            QIcon icon("Resources/pause.png");
            ui.player_play_pause_toggle_btn->setIcon(icon);
        }
    }
}


/*---------------------------------------------------------------------------------------------------------------------------
* FUNCTION:    audioPLayback()
*
* DATE:        11 Apr, 2014
*
* REVISIONS:   (Date and Description)
*              revision_date     revision_description
*
* DESIGNER:    Abhishek Bhardwaj
*
* PROGRAMMER:  Abhishek Bhardwaj
*
* RETURNS:     void
*
* NOTES:       Thread to playback audio...to unclutter the GUI thread.
--------------------------------------------------------------------------------------------------------------------------*/
void commAudio::audioPlayback()
{
    while(playerState == 1)
    {
        BASS_ChannelPlay(streamHandle, FALSE);
    }

    int err = BASS_ChannelPause(streamHandle);
    err = BASS_ErrorGetCode();
}

void commAudio::sendMessageButtonClick()
{
    if(!(ui.chat_message->toPlainText().isEmpty()))
    {
        addChatMessageToHistory("You", ui.chat_message->toPlainText());
        ui.chat_message->clear();
    }
}

void commAudio::addChatMessageToHistory(QString username, QString message)
{
    //QString::fromStdString(connectionSettings.getUsername())
    QString chatMessage = username + ": " + message + "\n";
    ui.chat_history->append(chatMessage);
}

void commAudio::pushToTalkButtonPressed()
{
    recording = true;

    BASS_ChannelPlay(micHandle, true);
}

void commAudio::pushToTalkButtonReleased()
{
    recording = false;

    BASS_ChannelPause(micHandle);
}

void commAudio::saveToFileCheckboxHandler()
{
    if(ui.checkBox_save_current_song->isChecked() == true)
    {
        saveFile = true;
    }
    else
    {
        saveFile = false;
    }
}

commAudio::~commAudio()
{
    if(playerState == 1)
    {
        BASS_ChannelPause(streamHandle);
    }

    if(connected == true)
    {
        clientNetwork.closeSocket(true);
        clientNetwork.closeSocket(false);
        connected = false;
    }

    BASS_Free();
    BASS_RecordFree();
    clientNetwork.terminateWinSock();
}
