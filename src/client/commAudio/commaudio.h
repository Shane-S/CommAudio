#ifndef COMMAUDIO_H
#define COMMAUDIO_H

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <WinSock2.h>
#include <Windows.h>

#define WM_SOCKET   (WM_USER + 1)

#include "ui_commaudio.h"
#include "ClientNetwork.h"
#include "bass.h"

class commAudio : public QMainWindow
{
    Q_OBJECT

public:
    commAudio(QWidget *parent = 0);
    ~commAudio();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    Ui::commAudioClass ui;
    ClientNetwork clientNetwork;
    int playerState; //0 = paused; 1 = playing
    HSTREAM streamHandle;
    HSTREAM streamBuffer;
    char dataBuffer[1024];

    private slots:
        void newConnectDialog();
        void newAudioUploadDialog();
        void playPauseButtonClick();
};

#endif // COMMAUDIO_H
