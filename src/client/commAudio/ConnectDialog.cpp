#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget * parent) : QDialog(parent)
{
    setupUi(this);
}

void ConnectDialog::getDataAndClose()
{
    QString temp;
    std::string temp1;
    int temp2;

    temp = ip_address->text();
    temp1 = temp.toStdString();
    connectionSettings.setIpAddress(temp1);

    temp = tcp_port_number->text();
    temp2 = temp.toInt();
    connectionSettings.setPortTCP(temp2);

    temp = udp_port_no->text();
    temp2 = temp.toInt();
    connectionSettings.setPortUDP(temp2);

    temp = username->text();
    temp1 = temp.toStdString();
    connectionSettings.setUsername(temp1);
    
    close();
}