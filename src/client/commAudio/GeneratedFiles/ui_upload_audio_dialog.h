/********************************************************************************
** Form generated from reading UI file 'upload_audio_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPLOAD_AUDIO_DIALOG_H
#define UI_UPLOAD_AUDIO_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_upload_audio_dialog
{
public:
    QLabel *label;
    QLineEdit *uploadDlg_audio_file_address;
    QPushButton *uploadDlg_open_file;
    QGroupBox *groupBox;
    QLabel *label_3;
    QLineEdit *uploadDlg_audio_name;
    QLineEdit *uploadDlg_artist_name;
    QLabel *label_4;
    QPushButton *uploadDlg_open_file_2;
    QLineEdit *uploadDlg_audio_art_address;
    QLabel *label_2;
    QProgressBar *uploadDlg_upload_progress_bar;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QWidget *upload_audio_dialog)
    {
        if (upload_audio_dialog->objectName().isEmpty())
            upload_audio_dialog->setObjectName(QStringLiteral("upload_audio_dialog"));
        upload_audio_dialog->resize(628, 359);
        label = new QLabel(upload_audio_dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 91, 31));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        uploadDlg_audio_file_address = new QLineEdit(upload_audio_dialog);
        uploadDlg_audio_file_address->setObjectName(QStringLiteral("uploadDlg_audio_file_address"));
        uploadDlg_audio_file_address->setGeometry(QRect(150, 20, 371, 31));
        uploadDlg_open_file = new QPushButton(upload_audio_dialog);
        uploadDlg_open_file->setObjectName(QStringLiteral("uploadDlg_open_file"));
        uploadDlg_open_file->setGeometry(QRect(520, 20, 81, 31));
        groupBox = new QGroupBox(upload_audio_dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 70, 571, 151));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 20, 111, 31));
        label_3->setFont(font);
        uploadDlg_audio_name = new QLineEdit(groupBox);
        uploadDlg_audio_name->setObjectName(QStringLiteral("uploadDlg_audio_name"));
        uploadDlg_audio_name->setGeometry(QRect(120, 20, 231, 31));
        uploadDlg_artist_name = new QLineEdit(groupBox);
        uploadDlg_artist_name->setObjectName(QStringLiteral("uploadDlg_artist_name"));
        uploadDlg_artist_name->setGeometry(QRect(120, 60, 231, 31));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 60, 111, 31));
        label_4->setFont(font);
        uploadDlg_open_file_2 = new QPushButton(groupBox);
        uploadDlg_open_file_2->setObjectName(QStringLiteral("uploadDlg_open_file_2"));
        uploadDlg_open_file_2->setGeometry(QRect(470, 100, 81, 31));
        uploadDlg_audio_art_address = new QLineEdit(groupBox);
        uploadDlg_audio_art_address->setObjectName(QStringLiteral("uploadDlg_audio_art_address"));
        uploadDlg_audio_art_address->setGeometry(QRect(120, 100, 351, 31));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 100, 91, 31));
        label_2->setFont(font);
        uploadDlg_upload_progress_bar = new QProgressBar(upload_audio_dialog);
        uploadDlg_upload_progress_bar->setObjectName(QStringLiteral("uploadDlg_upload_progress_bar"));
        uploadDlg_upload_progress_bar->setGeometry(QRect(30, 250, 571, 31));
        uploadDlg_upload_progress_bar->setValue(24);
        pushButton = new QPushButton(upload_audio_dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 300, 131, 41));
        pushButton_2 = new QPushButton(upload_audio_dialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(340, 300, 131, 41));
        pushButton_3 = new QPushButton(upload_audio_dialog);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setEnabled(false);
        pushButton_3->setGeometry(QRect(180, 300, 141, 41));
        pushButton_4 = new QPushButton(upload_audio_dialog);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(490, 300, 121, 41));

        retranslateUi(upload_audio_dialog);

        QMetaObject::connectSlotsByName(upload_audio_dialog);
    } // setupUi

    void retranslateUi(QWidget *upload_audio_dialog)
    {
        upload_audio_dialog->setWindowTitle(QApplication::translate("upload_audio_dialog", "Upload Audio", 0));
        label->setText(QApplication::translate("upload_audio_dialog", "Audio File:", 0));
        uploadDlg_open_file->setText(QApplication::translate("upload_audio_dialog", "Open", 0));
        groupBox->setTitle(QApplication::translate("upload_audio_dialog", "Audio File Information", 0));
        label_3->setText(QApplication::translate("upload_audio_dialog", "Audio Name:", 0));
        label_4->setText(QApplication::translate("upload_audio_dialog", "Artist Name:", 0));
        uploadDlg_open_file_2->setText(QApplication::translate("upload_audio_dialog", "Open", 0));
        label_2->setText(QApplication::translate("upload_audio_dialog", "Audio Art:", 0));
        pushButton->setText(QApplication::translate("upload_audio_dialog", "Start Upload", 0));
        pushButton_2->setText(QApplication::translate("upload_audio_dialog", "Clear", 0));
        pushButton_3->setText(QApplication::translate("upload_audio_dialog", "Upload in Background >>", 0));
        pushButton_4->setText(QApplication::translate("upload_audio_dialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class upload_audio_dialog: public Ui_upload_audio_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPLOAD_AUDIO_DIALOG_H
