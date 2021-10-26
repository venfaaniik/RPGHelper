#ifndef CONNECTION_TEST_H
#define CONNECTION_TEST_H

#include <QObject>

class connection_test : public QObject
{
    Q_OBJECT
public:
    explicit connection_test(QObject *parent = nullptr);

};

#endif // CONNECTION_TEST_H
