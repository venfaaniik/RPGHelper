#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = nullptr);
    void Connect();

    void sendToFaust(QString roll);

signals:

public slots:

private:
    QUdpSocket *socket;
};

#endif // MYUDP_H
