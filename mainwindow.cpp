#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TODO: FIND OUT HOW TO IGNORE OTHER THAN SKILL BUTTONS HERE
    QList<QPushButton*> pbList = findChildren<QPushButton*> (QString(), Qt::FindChildrenRecursively);
    foreach (QPushButton* pb, pbList) {
        //couldn't just yeet the lineedit list as an argument to the onSKillCLicked because cliked() is also empty. idk what stupidity is that.
        connect(pb, SIGNAL(clicked()), this, SLOT(onSkillClicked()));
        pb->setStyleSheet("Text-align:left");
    }
    connect(ui->CopyToClipBoard, SIGNAL(clicked()), this, SLOT(copyToClipboard()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QLineEdit *> MainWindow::getList()
{
    QList<QLineEdit*> leList = findChildren<QLineEdit*> (QString(), Qt::FindChildrenRecursively);
    return leList;
}

QString MainWindow::getBaseStat(QLineEdit* le)
{
    QString value;
    QString parent = le->parentWidget()->objectName();
    qDebug() << parent;

    //holyfuck jos jollain on parempi tapa tähä nii voi jakaa
    if (parent == "Group_INTELLIGENCE"){
        return value = ui->value_currentInt->text();
    } else if (parent == "Group_REFLEX"){
        return value = ui->value_currentReflex->text();
    } else if (parent == "Group_DEXTERITY"){
        return value = ui->value_currentDexterity->text();
    } else if (parent == "Group_BODY"){
        return value = ui->value_currentBody->text();
    } else if (parent == "Group_EMPATHY"){
        return value = ui->value_currentEmpathy->text();
    } else if (parent == "Group_CRAFTING"){
        return value = ui->value_currentCrafting->text();
    } else if (parent == "Group_WILL"){
        return value = ui->value_currentWill->text();
    }
    return "error!";
}

QString MainWindow::getModifiers()
{
    QString value;
    if (ui->WeddFed->isChecked() == true) {
        value = " + 1";
    }
    if (ui->WellRested->isChecked() == true) {
        value = value + " + 1";
    }
    if (ui->value_health->value() < 5) {
        //tähänki jos sais paremman tavan nii nice
        int helth = -1*(ui->value_health->value() - 5);
        //qDebug() << "Helth: " << helth;
        value = value + " - " + QString::number(helth);
    }
    return value;
}

void MainWindow::onSkillClicked()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    QString desiredName = (senderObjName + "_Value");
    qDebug() << "Button: " << senderObjName;

    QList<QLineEdit*> list = getList();

    foreach (QLineEdit* le, list) {
        if (le->objectName() == desiredName) {
            qDebug() << "Value: " << le->text();
            ui->Commandline->setText("!r " + getBaseStat(le) + " + " + le->text() + " + 1d10e1e10" + getModifiers());
            return;
        }
    }
}

void MainWindow::copyToClipboard()
{
    QString string = ui->Commandline->text();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(string);

    m.sendToFaust(string);
}

QString MainWindow::sendToBot()
{
    QString string = ui->Commandline->text();
    return string;
}
