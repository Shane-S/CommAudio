#include "commaudio.h"
#include "ConnectDialog.h"
#include "UploadAudioDialog.h"

commAudio::commAudio(QWidget *parent)
    : QMainWindow(parent)
{
    int freq = 44100;
    int device = -1;
    
    playerState = 0; //start player in paused state
    recording = false;

    BASS_Init(device, freq, 0, 0, NULL);
    BASS_RecordInit(device);

    streamHandle = BASS_StreamCreate(freq, 2, 0, STREAMPROC_PUSH, 0);
    micHandle = BASS_RecordStart(freq, 2, BASS_RECORD_PAUSE, NULL, NULL);

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
            case FD_CLOSE:
            {
                break;
            }
        }

        return true;
    }

    return false;
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
    //clientNetwork.sendPing();
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

    BASS_ChannelPause(streamHandle);
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

    std::thread record(&commAudio::startRecording, this);
    std::thread sendMicRecording(&commAudio::sendRecording, this);
    record.detach();
    sendMicRecording.detach();
}

void commAudio::pushToTalkButtonReleased()
{
    recording = false;
}

void commAudio::startRecording()
{
    int err;
    while(recording == true)
    {
        BASS_ChannelPlay(micHandle, false);
    }

    BASS_ChannelPause(micHandle);
}

void commAudio::sendRecording()
{
    while(recording == true)
    {
        clientNetwork.sendMicData(micHandle);
    }
}

commAudio::~commAudio()
{
    BASS_Free();
    BASS_RecordFree();
    clientNetwork.terminateWinSock();
}
