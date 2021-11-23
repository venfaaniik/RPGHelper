#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "grimoiretest.h"

#include "myudp.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initTables();
    void setSkill(QString s);
    QString getSkill();

    QList<QLineEdit*> getList();
    QString getBaseStat(QLineEdit* le);
    QString getModifiers();
    QString sendToBot();

private slots:
    void onSkillClicked();
    void onCellClicked(int index);
    void copyToClipboard();
    void healthChanged();
    void resetLuck();
    void saveTo();
    void loadFrom();
    void basicDice();

    void changeWindow();
    void showGrimoire();

private:
    Ui::MainWindow *ui;
    Grimoiretest *grimoire;
    MyUDP m;
    QMap<QString, QString> savingValues;

    QString *skill;
};
#endif // MAINWINDOW_H
