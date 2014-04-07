#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget * parent) : QDialog(parent)
{
    setupUi(this);
}

void ConnectDialog::getDataAndClose()
{
    connectionSettings.setIpAddress(ip_address->text().toStdString());
    connectionSettings.setPortTCP(tcp_port_number->text().toInt());
    connectionSettings.setPortUDP(udp_port_no->text().toInt());
    connectionSettings.setUsername(username->text().toStdString());
    
    close();
}