#include "commaudio.h"
#include "ConnectDialog.h"
#include "UploadAudioDialog.h"

commAudio::commAudio(QWidget *parent)
    : QMainWindow(parent)
{
    
    playerState = 0; //start player in paused state
    recording = false;
    connected = false;
    checkHandshake = 0;

    BASS_Init(-1, 44100, 0, 0, NULL);
    BASS_RecordInit(-1);

    clientNetwork.setHWND((HWND)this->winId());
    ui.setupUi(this);
}

bool commAudio::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* recvMessage = reinterpret_cast<MSG*>(message);
 
    WSABUF buffer;
    DWORD bytesReceived;
    DWORD flags;
    static int totalBytesRecv;
    char dataBuffer[2048];

    //PAUDIOPLAYBACKDATA audioPlaybackData = (PAUDIOPLAYBACKDATA) malloc(sizeof(PAUDIOPLAYBACKDATA));

    if(recvMessage->message == WM_SOCKET)
    {
        switch(WSAGETSELECTEVENT(recvMessage->lParam))
        {
            case FD_READ:
            {
                flags = 0;
                buffer.buf = (CHAR*)dataBuffer;
                buffer.len = 2048;
                int err = WSARecv(clientNetwork.getTCPSocket(), &buffer, 1, &bytesReceived, &flags, NULL, NULL);
            
                if(bytesReceived > 0)
                {
                    /**if(playerState == 0)
                    {
                        playerState = 1;
                    }*/
                    err = BASS_StreamPutData(streamHandle, dataBuffer, bytesReceived);
                    //err = BASS_ErrorGetCode();
                }

                break;
            }
            case FD_WRITE:
            {
                connected = true;
                
                if(checkHandshake == 0)
                {
                    clientNetwork.sendPing();
                    checkHandshake = 1;
                }
                break;
            }
            case FD_CLOSE:
            {
                connected = false;
                checkHandshake = 0;
                break;
            }
        }

        return true;
    }

    return false;
}

BOOL CALLBACK MyRecordProc(HRECORD handle, const void *buffer, DWORD length, void *user)
{
    DWORD SendBytes = 0;
    DWORD BytesTransferred = 0;
    WSABUF WSbuffer;
	int err;
	char * buf = (char*)buffer;
    PCONNECTIONSTRUCT cStruct = (PCONNECTIONSTRUCT) user;

	int pos = 0;
	while(length > pos)
	{
		WSbuffer.len = AUDIO_BUFFER_LENGTH;
		WSbuffer.buf = (char*)buf + pos;

        err = WSASendTo(cStruct->UDPSocket , &WSbuffer, 1, &SendBytes, 0, (sockaddr*)&cStruct->UDPSockAddr , sizeof(cStruct->UDPSockAddr), NULL, NULL);

		pos += AUDIO_BUFFER_LENGTH;

        Sleep(SLEEP_DURATION);
	}
    return true; // continue recording
}

void commAudio::newConnectDialog()
{
    playerState = 0; //start player in paused state

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

void commAudio::newAudioUploadDialog()
{
    UploadAudioDialog *uploadAudioDialog = new UploadAudioDialog;
    uploadAudioDialog->show();
}

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

commAudio::~commAudio()
{
    BASS_Free();
    BASS_RecordFree();
    clientNetwork.terminateWinSock();
}
