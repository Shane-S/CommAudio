/********************************************************************************
** Form generated from reading UI file 'commaudio.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMAUDIO_H
#define UI_COMMAUDIO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_commAudioClass
{
public:
    QAction *actionUser_Guide;
    QAction *actionProgram_Documentation;
    QAction *actionAbout_Program;
    QAction *actionAbout_Authors;
    QAction *actionConnect_to_Server;
    QAction *actionMulticast;
    QAction *actionSave_Playlist;
    QAction *actionSave_Current_Song;
    QAction *actionOpen_Playlist;
    QAction *actionUpload_a_Song;
    QAction *actionExit;
    QAction *actionNormal_Playlist_Mode;
    QAction *actionMulticast_Radio;
    QAction *actionNormal_Radio;
    QAction *actionMulticast_Radio_2;
    QWidget *centralWidget;
    QTableWidget *song_list_on_server;
    QGroupBox *groupBox;
    QSlider *player_volume;
    QLabel *player_audio_duration;
    QSlider *player_seekbar;
    QGraphicsView *player_audio_pic;
    QLabel *player_audio_name;
    QPushButton *player_play_pause_toggle_btn;
    QPushButton *player_next_btn;
    QPushButton *player_previous_btn;
    QPushButton *pushButton;
    QCheckBox *checkBox_save_current_song;
    QFrame *line;
    QLabel *label;
    QLabel *label_2;
    QTextEdit *chat_history;
    QPlainTextEdit *chat_message;
    QPushButton *send_chat_message_btn;
    QPushButton *push_to_talk_btn;
    QTableWidget *playlist_client;
    QLabel *label_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuMode;
    QMenu *menuRadio;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *commAudioClass)
    {
        if (commAudioClass->objectName().isEmpty())
            commAudioClass->setObjectName(QStringLiteral("commAudioClass"));
        commAudioClass->resize(771, 590);
        actionUser_Guide = new QAction(commAudioClass);
        actionUser_Guide->setObjectName(QStringLiteral("actionUser_Guide"));
        actionProgram_Documentation = new QAction(commAudioClass);
        actionProgram_Documentation->setObjectName(QStringLiteral("actionProgram_Documentation"));
        actionAbout_Program = new QAction(commAudioClass);
        actionAbout_Program->setObjectName(QStringLiteral("actionAbout_Program"));
        actionAbout_Authors = new QAction(commAudioClass);
        actionAbout_Authors->setObjectName(QStringLiteral("actionAbout_Authors"));
        actionConnect_to_Server = new QAction(commAudioClass);
        actionConnect_to_Server->setObjectName(QStringLiteral("actionConnect_to_Server"));
        actionMulticast = new QAction(commAudioClass);
        actionMulticast->setObjectName(QStringLiteral("actionMulticast"));
        actionSave_Playlist = new QAction(commAudioClass);
        actionSave_Playlist->setObjectName(QStringLiteral("actionSave_Playlist"));
        actionSave_Playlist->setEnabled(false);
        actionSave_Current_Song = new QAction(commAudioClass);
        actionSave_Current_Song->setObjectName(QStringLiteral("actionSave_Current_Song"));
        actionSave_Current_Song->setEnabled(false);
        actionOpen_Playlist = new QAction(commAudioClass);
        actionOpen_Playlist->setObjectName(QStringLiteral("actionOpen_Playlist"));
        actionOpen_Playlist->setEnabled(false);
        actionUpload_a_Song = new QAction(commAudioClass);
        actionUpload_a_Song->setObjectName(QStringLiteral("actionUpload_a_Song"));
        actionExit = new QAction(commAudioClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionNormal_Playlist_Mode = new QAction(commAudioClass);
        actionNormal_Playlist_Mode->setObjectName(QStringLiteral("actionNormal_Playlist_Mode"));
        actionNormal_Playlist_Mode->setEnabled(false);
        actionMulticast_Radio = new QAction(commAudioClass);
        actionMulticast_Radio->setObjectName(QStringLiteral("actionMulticast_Radio"));
        actionNormal_Radio = new QAction(commAudioClass);
        actionNormal_Radio->setObjectName(QStringLiteral("actionNormal_Radio"));
        actionNormal_Radio->setEnabled(false);
        actionMulticast_Radio_2 = new QAction(commAudioClass);
        actionMulticast_Radio_2->setObjectName(QStringLiteral("actionMulticast_Radio_2"));
        centralWidget = new QWidget(commAudioClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        song_list_on_server = new QTableWidget(centralWidget);
        if (song_list_on_server->columnCount() < 5)
            song_list_on_server->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        song_list_on_server->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        song_list_on_server->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        song_list_on_server->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        song_list_on_server->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        song_list_on_server->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        song_list_on_server->setObjectName(QStringLiteral("song_list_on_server"));
        song_list_on_server->setGeometry(QRect(10, 40, 511, 181));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 390, 511, 131));
        player_volume = new QSlider(groupBox);
        player_volume->setObjectName(QStringLiteral("player_volume"));
        player_volume->setGeometry(QRect(320, 100, 131, 20));
        player_volume->setOrientation(Qt::Horizontal);
        player_audio_duration = new QLabel(groupBox);
        player_audio_duration->setObjectName(QStringLiteral("player_audio_duration"));
        player_audio_duration->setGeometry(QRect(460, 58, 61, 41));
        QFont font;
        font.setPointSize(15);
        player_audio_duration->setFont(font);
        player_seekbar = new QSlider(groupBox);
        player_seekbar->setObjectName(QStringLiteral("player_seekbar"));
        player_seekbar->setGeometry(QRect(110, 68, 341, 22));
        player_seekbar->setOrientation(Qt::Horizontal);
        player_audio_pic = new QGraphicsView(groupBox);
        player_audio_pic->setObjectName(QStringLiteral("player_audio_pic"));
        player_audio_pic->setGeometry(QRect(10, 28, 81, 91));
        player_audio_name = new QLabel(groupBox);
        player_audio_name->setObjectName(QStringLiteral("player_audio_name"));
        player_audio_name->setGeometry(QRect(110, 20, 381, 31));
        QFont font1;
        font1.setPointSize(12);
        player_audio_name->setFont(font1);
        player_play_pause_toggle_btn = new QPushButton(groupBox);
        player_play_pause_toggle_btn->setObjectName(QStringLiteral("player_play_pause_toggle_btn"));
        player_play_pause_toggle_btn->setGeometry(QRect(150, 100, 30, 20));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        player_play_pause_toggle_btn->setIcon(icon);
        player_next_btn = new QPushButton(groupBox);
        player_next_btn->setObjectName(QStringLiteral("player_next_btn"));
        player_next_btn->setGeometry(QRect(190, 100, 30, 20));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        player_next_btn->setIcon(icon1);
        player_previous_btn = new QPushButton(groupBox);
        player_previous_btn->setObjectName(QStringLiteral("player_previous_btn"));
        player_previous_btn->setGeometry(QRect(110, 100, 30, 20));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        player_previous_btn->setIcon(icon2);
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(290, 100, 30, 20));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Resources/volume.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon3);
        checkBox_save_current_song = new QCheckBox(groupBox);
        checkBox_save_current_song->setObjectName(QStringLiteral("checkBox_save_current_song"));
        checkBox_save_current_song->setGeometry(QRect(410, 10, 91, 20));
        QFont font2;
        font2.setPointSize(8);
        checkBox_save_current_song->setFont(font2);
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(530, 10, 20, 511));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 131, 31));
        QFont font3;
        font3.setPointSize(14);
        label->setFont(font3);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(560, 10, 131, 31));
        label_2->setFont(font3);
        chat_history = new QTextEdit(centralWidget);
        chat_history->setObjectName(QStringLiteral("chat_history"));
        chat_history->setEnabled(true);
        chat_history->setGeometry(QRect(560, 50, 201, 301));
        chat_history->setAutoFillBackground(false);
        chat_history->setReadOnly(true);
        chat_message = new QPlainTextEdit(centralWidget);
        chat_message->setObjectName(QStringLiteral("chat_message"));
        chat_message->setGeometry(QRect(560, 360, 201, 81));
        send_chat_message_btn = new QPushButton(centralWidget);
        send_chat_message_btn->setObjectName(QStringLiteral("send_chat_message_btn"));
        send_chat_message_btn->setGeometry(QRect(560, 450, 201, 31));
        QIcon icon4;
        icon4.addFile(QStringLiteral("Resources/chat.png"), QSize(), QIcon::Normal, QIcon::Off);
        send_chat_message_btn->setIcon(icon4);
        push_to_talk_btn = new QPushButton(centralWidget);
        push_to_talk_btn->setObjectName(QStringLiteral("push_to_talk_btn"));
        push_to_talk_btn->setGeometry(QRect(560, 490, 201, 31));
        QIcon icon5;
        icon5.addFile(QStringLiteral("Resources/microphone.png"), QSize(), QIcon::Normal, QIcon::Off);
        push_to_talk_btn->setIcon(icon5);
        playlist_client = new QTableWidget(centralWidget);
        if (playlist_client->columnCount() < 5)
            playlist_client->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        playlist_client->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        playlist_client->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        playlist_client->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        playlist_client->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        playlist_client->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        playlist_client->setObjectName(QStringLiteral("playlist_client"));
        playlist_client->setGeometry(QRect(10, 260, 511, 121));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 230, 131, 31));
        label_3->setFont(font3);
        commAudioClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(commAudioClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 771, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuMode = new QMenu(menuBar);
        menuMode->setObjectName(QStringLiteral("menuMode"));
        menuRadio = new QMenu(menuMode);
        menuRadio->setObjectName(QStringLiteral("menuRadio"));
        commAudioClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(commAudioClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        commAudioClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(commAudioClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        commAudioClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuMode->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionConnect_to_Server);
        menuFile->addSeparator();
        menuFile->addAction(actionOpen_Playlist);
        menuFile->addAction(actionUpload_a_Song);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Playlist);
        menuFile->addAction(actionSave_Current_Song);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionUser_Guide);
        menuHelp->addAction(actionProgram_Documentation);
        menuHelp->addAction(actionAbout_Program);
        menuHelp->addAction(actionAbout_Authors);
        menuMode->addAction(actionNormal_Playlist_Mode);
        menuMode->addSeparator();
        menuMode->addAction(menuRadio->menuAction());
        menuRadio->addAction(actionNormal_Radio);
        menuRadio->addAction(actionMulticast_Radio_2);

        retranslateUi(commAudioClass);
        QObject::connect(actionConnect_to_Server, SIGNAL(triggered()), commAudioClass, SLOT(newConnectDialog()));
        QObject::connect(actionUpload_a_Song, SIGNAL(triggered()), commAudioClass, SLOT(newAudioUploadDialog()));
        QObject::connect(actionExit, SIGNAL(triggered()), commAudioClass, SLOT(close()));
        QObject::connect(player_play_pause_toggle_btn, SIGNAL(clicked()), commAudioClass, SLOT(playPauseButtonClick()));
        QObject::connect(send_chat_message_btn, SIGNAL(clicked()), commAudioClass, SLOT(sendMessageButtonClick()));
        QObject::connect(push_to_talk_btn, SIGNAL(pressed()), commAudioClass, SLOT(pushToTalkButtonPressed()));
        QObject::connect(push_to_talk_btn, SIGNAL(released()), commAudioClass, SLOT(pushToTalkButtonReleased()));
        QObject::connect(checkBox_save_current_song, SIGNAL(stateChanged(int)), commAudioClass, SLOT(saveToFileCheckboxHandler()));
        QObject::connect(actionMulticast_Radio_2, SIGNAL(triggered()), commAudioClass, SLOT(multicastSwitch()));

        QMetaObject::connectSlotsByName(commAudioClass);
    } // setupUi

    void retranslateUi(QMainWindow *commAudioClass)
    {
        commAudioClass->setWindowTitle(QApplication::translate("commAudioClass", "commAudio", 0));
        actionUser_Guide->setText(QApplication::translate("commAudioClass", "User Guide", 0));
        actionProgram_Documentation->setText(QApplication::translate("commAudioClass", "Program Documentation", 0));
        actionAbout_Program->setText(QApplication::translate("commAudioClass", "About Program", 0));
        actionAbout_Authors->setText(QApplication::translate("commAudioClass", "About Authors", 0));
        actionConnect_to_Server->setText(QApplication::translate("commAudioClass", "Connect", 0));
        actionMulticast->setText(QApplication::translate("commAudioClass", "Multicast", 0));
        actionSave_Playlist->setText(QApplication::translate("commAudioClass", "Save Playlist", 0));
        actionSave_Current_Song->setText(QApplication::translate("commAudioClass", "Save Current Song", 0));
        actionOpen_Playlist->setText(QApplication::translate("commAudioClass", "Open Playlist", 0));
        actionUpload_a_Song->setText(QApplication::translate("commAudioClass", "Upload a Song", 0));
        actionExit->setText(QApplication::translate("commAudioClass", "Exit", 0));
        actionNormal_Playlist_Mode->setText(QApplication::translate("commAudioClass", "Normal Mode", 0));
        actionMulticast_Radio->setText(QApplication::translate("commAudioClass", "Multicast Radio", 0));
        actionNormal_Radio->setText(QApplication::translate("commAudioClass", "Normal Radio", 0));
        actionMulticast_Radio_2->setText(QApplication::translate("commAudioClass", "Multicast Radio", 0));
        QTableWidgetItem *___qtablewidgetitem = song_list_on_server->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("commAudioClass", "Art", 0));
        QTableWidgetItem *___qtablewidgetitem1 = song_list_on_server->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("commAudioClass", "Song", 0));
        QTableWidgetItem *___qtablewidgetitem2 = song_list_on_server->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("commAudioClass", "Artist", 0));
        QTableWidgetItem *___qtablewidgetitem3 = song_list_on_server->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("commAudioClass", "Album", 0));
        QTableWidgetItem *___qtablewidgetitem4 = song_list_on_server->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("commAudioClass", "Year", 0));
        groupBox->setTitle(QApplication::translate("commAudioClass", "Player", 0));
        player_audio_duration->setText(QApplication::translate("commAudioClass", "2:40", 0));
        player_audio_name->setText(QApplication::translate("commAudioClass", "Song name", 0));
        player_play_pause_toggle_btn->setText(QString());
        player_next_btn->setText(QString());
        player_previous_btn->setText(QString());
        pushButton->setText(QString());
        checkBox_save_current_song->setText(QApplication::translate("commAudioClass", "Save this song", 0));
        label->setText(QApplication::translate("commAudioClass", "Song List:", 0));
        label_2->setText(QApplication::translate("commAudioClass", "Chat:", 0));
        send_chat_message_btn->setText(QApplication::translate("commAudioClass", "Send Message", 0));
        push_to_talk_btn->setText(QApplication::translate("commAudioClass", "Push To Talk", 0));
        QTableWidgetItem *___qtablewidgetitem5 = playlist_client->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QApplication::translate("commAudioClass", "Art", 0));
        QTableWidgetItem *___qtablewidgetitem6 = playlist_client->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QApplication::translate("commAudioClass", "Song", 0));
        QTableWidgetItem *___qtablewidgetitem7 = playlist_client->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QApplication::translate("commAudioClass", "Artist", 0));
        QTableWidgetItem *___qtablewidgetitem8 = playlist_client->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QApplication::translate("commAudioClass", "Album", 0));
        QTableWidgetItem *___qtablewidgetitem9 = playlist_client->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QApplication::translate("commAudioClass", "Year", 0));
        label_3->setText(QApplication::translate("commAudioClass", "Playlist:", 0));
        menuFile->setTitle(QApplication::translate("commAudioClass", "File", 0));
        menuHelp->setTitle(QApplication::translate("commAudioClass", "Help", 0));
        menuMode->setTitle(QApplication::translate("commAudioClass", "Mode", 0));
        menuRadio->setTitle(QApplication::translate("commAudioClass", "Radio", 0));
    } // retranslateUi

};

namespace Ui {
    class commAudioClass: public Ui_commAudioClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMAUDIO_H
