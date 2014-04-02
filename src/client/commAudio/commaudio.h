#ifndef COMMAUDIO_H
#define COMMAUDIO_H

#include <QtWidgets/QMainWindow>
#include "ui_commaudio.h"

class commAudio : public QMainWindow
{
    Q_OBJECT

public:
    commAudio(QWidget *parent = 0);
    ~commAudio();

private:
    Ui::commAudioClass ui;
};

#endif // COMMAUDIO_H
