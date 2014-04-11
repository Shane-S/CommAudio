#ifndef COMMAUDIO_H
#define COMMAUDIO_H

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <WinSock2.h>
#include <thread>
#include <Windows.h>

#define WM_SOCKET   (WM_USER + 1)
#define AUDIO_BUFFER_LENGTH 4096 //for mic
#define STREAM_BUFFER_LENGTH 2048 //for audio streaming

#include "ui_commaudio.h"
#include "ClientNetwork.h"
#include "ConnectionSettings.h"
#include "bass.h"

typedef struct connectionStruct
{
	SOCKET UDPSocket;
	SOCKADDR_IN UDPSockAddr;
} *PCONNECTIONSTRUCT, connectionStruct;

typedef struct audioReceiveTCPStruct
{
    WSAOVERLAPPED fakeOverlapped;
    HSTREAM streamH;
    WSABUF buffer;
} *PAUDIORECEIVESTRUCT, audioReceiveTCPStruct;

class commAudio : public QMainWindow
{
    Q_OBJECT

public:
    commAudio(QWidget *parent = 0);
    ~commAudio();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void addChatMessageToHistory(QString username, QString message);
    void audioPlayback();

private:
    Ui::commAudioClass  ui;
    ClientNetwork       clientNetwork;
    ConnectionSettings  connectionSettings;
    int                 playerState; //0 = paused; 1 = playing
    bool                connected;
    int                 checkHandshake;
    HSTREAM             streamHandle;
    HRECORD             micHandle;
    bool                recording; //false if not current recording, true if recording

    private slots:
        void newConnectDialog();
        void newAudioUploadDialog();
        void playPauseButtonClick();
        void sendMessageButtonClick();
        void pushToTalkButtonPressed();
        void pushToTalkButtonReleased();
        void saveToFileCheckboxHandler();
};

#endif // COMMAUDIO_H
