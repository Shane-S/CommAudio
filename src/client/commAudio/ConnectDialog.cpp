/*--------------------------------------------------------------------------------------------------------------------
* SOURCE FILE:     ConnectDialog - File for connect dialog
*
* PROGRAM:         commAudio
*
* FUNCTIONS:       list_of_functions
*
* DATE:            11 April 2014
*
* REVISIONS:       (Date and Description)
*                  revision_date     revision_description
*
* DESIGNER:        Abhishek Bhardwaj
*
* PROGRAMMER:      Abhishek Bhardwaj
*
* NOTES:           File for connect dialog.
*
------------------------------------------------------------------------------------------------------------------------*/

#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget * parent) : QDialog(parent)
{
    setupUi(this);
}

//gets data from the dialog box variables..sets them to an object which can then be used in other files
void ConnectDialog::getDataAndClose()
{
    connectionSettings.setIpAddress(ip_address->text().toStdString());
    connectionSettings.setPortTCP(tcp_port_number->text().toInt());
    connectionSettings.setPortUDP(udp_port_no->text().toInt());
    connectionSettings.setUsername(username->text().toStdString());
    
    close();
}