#ifndef GRIMOIRETEST_H
#define GRIMOIRETEST_H

#include <QMainWindow>
#include <qtablewidget.h>

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
    void addToTable();
    void removeFromTable();
    void sort();

private:
    Ui::Grimoiretest *ui;
    QTableWidget *spellTable;
};

#endif // GRIMOIRETEST_H
