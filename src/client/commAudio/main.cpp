#include "commaudio.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    commAudio w;
    w.show();
    return a.exec();
}
