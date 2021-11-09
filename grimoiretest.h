#ifndef GRIMOIRETEST_H
#define GRIMOIRETEST_H

#include <QMainWindow>

namespace Ui {
class Grimoiretest;
}

class Grimoiretest : public QMainWindow
{
    Q_OBJECT

public:
    explicit Grimoiretest(QWidget *parent = nullptr);
    ~Grimoiretest();

public slots:
    void addToList();

private:
    Ui::Grimoiretest *ui;
};

#endif // GRIMOIRETEST_H
