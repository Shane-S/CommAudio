#ifndef UPLOAD_AUDIO_DIALOG_H
#define UPLOAD_AUDIO_DIALOG_H

#include <QDialog>
#include "ui_upload_audio_dialog.h"

class UploadAudioDialog : public QDialog, public Ui::upload_audio_dialog
{
    Q_OBJECT

public:
    UploadAudioDialog(QWidget *parent = 0);
};

#endif