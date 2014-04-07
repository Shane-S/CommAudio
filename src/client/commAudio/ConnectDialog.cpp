#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget * parent) : QDialog(parent)
{
    setupUi(this);
}

void ConnectDialog::getDataAndClose()
{
    if(ui.ip_address->text().isNull() == false)
    {
        connectionSettings.setIpAddress(ui.ip_address->text().toStdString());
    }

    if(ui.tcp_port_number->text().isNull() == false)
    {
        connectionSettings.setPortTCP(ui.tcp_port_number->text().toInt());
    }

    if(ui.udp_port_no->text().isNull() == false)
    {
        connectionSettings.setPortUDP(ui.udp_port_no->text().toInt());
    }

    if(ui.username->text().isNull() == false)
    {
        connectionSettings.setUsername(ui.username->text().toStdString());
    }

    close();
}