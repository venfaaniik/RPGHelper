#include "mainwindow.h"
#include "myudp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MyUDP client;
    //client.Connect();

    return a.exec();
}
