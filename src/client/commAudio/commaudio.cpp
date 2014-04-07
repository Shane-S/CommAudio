#include "commaudio.h"
#include "ConnectDialog.h"
#include "UploadAudioDialog.h"

commAudio::commAudio(QWidget *parent)
    : QMainWindow(parent)
{
    int freq = 44100;
    int device = -1;

    playerState = 0; //start player in paused state
    
    BASS_Init(device, freq, 0, 0, NULL);
    streamHandle = BASS_StreamCreate(freq, 2, 0, STREAMPROC_PUSH, 0);

    ui.setupUi(this);
}

bool commAudio::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* recvMessage = reinterpret_cast<MSG*>(message);
 
    WSABUF buffer;
    DWORD bytesReceived, flags;
    static int totalBytesRecv;
    char dataBuffer[2048];

    if(recvMessage->message == WM_SOCKET)
    {
        switch(WSAGETSELECTEVENT(recvMessage->lParam))
        {
        case FD_READ:
            flags = 0;
            buffer.buf = (CHAR*)dataBuffer;
            buffer.len = sizeof(char) * 10000;
            WSARecv(clientNetwork.getTCPSocket(), &buffer, 1, &bytesReceived, &flags, NULL, NULL);
            
            if(bytesReceived > 0)
            {
                if(playerState == 0)
                {
                    playerState = 1;
                }
                BASS_StreamPutData(streamHandle, dataBuffer, bytesReceived);
            }

            break;
        }

        return true;
    }

    return false;
}

void commAudio::newConnectDialog()
{
    ConnectDialog *connectDialog = new ConnectDialog;
    //connectDialog->show();
    connectDialog->exec();
    
    connectionSettings = connectDialog->connectionSettings;

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

            BASS_ChannelPlay(streamHandle, FALSE);

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

commAudio::~commAudio()
{
    BASS_Free();
}
