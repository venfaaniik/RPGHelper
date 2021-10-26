#include "myudp.h"
#include "mainwindow.h"

MyUDP::MyUDP(QObject *parent) : QObject(parent)
{

}

void MyUDP::Connect()
{
    MainWindow w;
    socket = new QUdpSocket(this);
    socket->connectToHost("127.0.0.1", 8080);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        QString text = w.sendToBot();
        QByteArray textUtf8 = text.toUtf8();
        socket->write(textUtf8);
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

        socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }

    // sent

    // got

    // closed
}

void MyUDP::sendToFaust(QString roll)
{
    MainWindow w;
    socket = new QUdpSocket(this);
    socket->connectToHost("127.0.0.1", 8080);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        // send
        QString text = roll;
        qDebug() << text;
        QByteArray textUtf8 = text.toUtf8();
        socket->write(textUtf8);

        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

        socket->close();
    }
    else
    {
        qDebug() << "Not connected!";
    }
}
