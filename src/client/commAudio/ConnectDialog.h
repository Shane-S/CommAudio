#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QDialog>
#include "ui_connect_dialog.h"
#include "ConnectionSettings.h"

class ConnectDialog : public QDialog, public Ui::connect_dialog
{
    Q_OBJECT

public:
    ConnectDialog(QWidget *parent = 0);
    ConnectionSettings connectionSettings;

private:

private slots:
    void getDataAndClose();

};

#endif