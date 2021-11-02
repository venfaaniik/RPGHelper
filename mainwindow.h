#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>

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

    QList<QLineEdit*> getList();
    QString getBaseStat(QLineEdit* le);
    QString getModifiers();
    QString sendToBot();

private slots:
    void onSkillClicked();
    void copyToClipboard();
    void healthChanged();
    void resetLuck();
    void saveTo();
    void loadFrom();
    void basicDice();

private:
    Ui::MainWindow *ui;
    MyUDP m;
    QMap<QString, QString> savingValues;
};
#endif // MAINWINDOW_H
