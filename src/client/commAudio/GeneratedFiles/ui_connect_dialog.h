/********************************************************************************
** Form generated from reading UI file 'connect_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECT_DIALOG_H
#define UI_CONNECT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_connect_dialog
{
public:
    QLabel *label;
    QLineEdit *ip_address;
    QLineEdit *tcp_port_number;
    QLabel *label_2;
    QPushButton *connectDlg_connect_btn;
    QPushButton *connectDlg_cancel_btn;
    QLineEdit *username;
    QLabel *label_3;
    QFrame *line;
    QLineEdit *udp_port_no;
    QLabel *label_4;

    void setupUi(QWidget *connect_dialog)
    {
        if (connect_dialog->objectName().isEmpty())
            connect_dialog->setObjectName(QStringLiteral("connect_dialog"));
        connect_dialog->resize(360, 291);
        label = new QLabel(connect_dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 81, 31));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        ip_address = new QLineEdit(connect_dialog);
        ip_address->setObjectName(QStringLiteral("ip_address"));
        ip_address->setGeometry(QRect(140, 20, 201, 31));
        QFont font1;
        font1.setPointSize(10);
        ip_address->setFont(font1);
        tcp_port_number = new QLineEdit(connect_dialog);
        tcp_port_number->setObjectName(QStringLiteral("tcp_port_number"));
        tcp_port_number->setGeometry(QRect(140, 70, 101, 31));
        tcp_port_number->setFont(font1);
        label_2 = new QLabel(connect_dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 70, 101, 31));
        label_2->setFont(font);
        connectDlg_connect_btn = new QPushButton(connect_dialog);
        connectDlg_connect_btn->setObjectName(QStringLiteral("connectDlg_connect_btn"));
        connectDlg_connect_btn->setGeometry(QRect(70, 240, 81, 31));
        connectDlg_cancel_btn = new QPushButton(connect_dialog);
        connectDlg_cancel_btn->setObjectName(QStringLiteral("connectDlg_cancel_btn"));
        connectDlg_cancel_btn->setGeometry(QRect(200, 240, 81, 31));
        username = new QLineEdit(connect_dialog);
        username->setObjectName(QStringLiteral("username"));
        username->setGeometry(QRect(140, 190, 201, 31));
        username->setFont(font1);
        label_3 = new QLabel(connect_dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 190, 81, 31));
        label_3->setFont(font);
        line = new QFrame(connect_dialog);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(30, 160, 311, 21));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        udp_port_no = new QLineEdit(connect_dialog);
        udp_port_no->setObjectName(QStringLiteral("udp_port_no"));
        udp_port_no->setGeometry(QRect(140, 120, 101, 31));
        udp_port_no->setFont(font1);
        label_4 = new QLabel(connect_dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 120, 101, 31));
        label_4->setFont(font);
        QWidget::setTabOrder(ip_address, tcp_port_number);
        QWidget::setTabOrder(tcp_port_number, udp_port_no);
        QWidget::setTabOrder(udp_port_no, username);
        QWidget::setTabOrder(username, connectDlg_connect_btn);
        QWidget::setTabOrder(connectDlg_connect_btn, connectDlg_cancel_btn);

        retranslateUi(connect_dialog);
        QObject::connect(connectDlg_connect_btn, SIGNAL(clicked()), connect_dialog, SLOT(getDataAndClose()));
        QObject::connect(connectDlg_cancel_btn, SIGNAL(clicked()), connect_dialog, SLOT(close()));

        QMetaObject::connectSlotsByName(connect_dialog);
    } // setupUi

    void retranslateUi(QWidget *connect_dialog)
    {
        connect_dialog->setWindowTitle(QApplication::translate("connect_dialog", "Connect to Server", 0));
        label->setText(QApplication::translate("connect_dialog", "Server IP:", 0));
        ip_address->setText(QApplication::translate("connect_dialog", "192.168.43.100", 0));
        tcp_port_number->setText(QApplication::translate("connect_dialog", "7000", 0));
        label_2->setText(QApplication::translate("connect_dialog", "TCP Port No.:", 0));
        connectDlg_connect_btn->setText(QApplication::translate("connect_dialog", "Connect", 0));
        connectDlg_cancel_btn->setText(QApplication::translate("connect_dialog", "Cancel", 0));
        username->setText(QApplication::translate("connect_dialog", "Abhishek", 0));
        label_3->setText(QApplication::translate("connect_dialog", "Username:", 0));
        udp_port_no->setText(QApplication::translate("connect_dialog", "7001", 0));
        label_4->setText(QApplication::translate("connect_dialog", "UDP Port No.:", 0));
    } // retranslateUi

};

namespace Ui {
    class connect_dialog: public Ui_connect_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECT_DIALOG_H
