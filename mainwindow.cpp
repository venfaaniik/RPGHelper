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
        //couldn't just yeet the lineedit list as an argument to the onSKillCLicked because cliked() is also empty. idk what kind of stupidity is that.
        connect(pb, SIGNAL(clicked()), this, SLOT(onSkillClicked()));
        pb->setStyleSheet("Text-align:left");
    }
    connect(ui->CopyToClipBoard, SIGNAL(clicked()), this, SLOT(copyToClipboard()));
    connect(ui->value_health, SIGNAL(valueChanged(int)), this, SLOT(healthChanged()));
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
    //a little if-mess, as a treat
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
        return value = ui->value_currentCrafting2->text();
    } else if (parent == "Group_WILL"){
        return value = ui->value_currentWill->text();
    }
    return "error";
}

QString MainWindow::getModifiers()
{
    QString value;
    if (ui->WeddFed->isChecked() && ui->WellRested->isChecked() == true) {
        value = " + 2 [WellFed | WellRested]";
    } else if (ui->WeddFed->isChecked() == true) {
        value = " + 1 [WellFed]";
    } else if (ui->WellRested->isChecked() == true) {
        value = value + " + 1 [WellRested]";
    }

    if (ui->Other_Buffs->value() != 0) {
        value = value + " + " + ui->Other_Buffs->text();
    }

    if (ui->value_health->value() < 5) {
        int helth = -1 *(ui->value_health->value() - 5);
        value = value + " - " + QString::number(helth);
    }

    return value;
}

void MainWindow::onSkillClicked()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    QString skillName = (senderObjName + "_Value");
    QString attributeName = ("value_current" + senderObjName);
    qDebug() << "Button: " << senderObjName;

    QList<QLineEdit*> list = getList();

    foreach (QLineEdit* le, list) {
       if (le->objectName() == attributeName) {
            qDebug() << "Value: " << le->text();
            ui->Commandline->setText("!r " + le->text() + " + 1d10e1e10" + getModifiers());
            return;
       } else if (le->objectName() == skillName) {
           qDebug() << "Value: " << le->text();
           if (le->text() != nullptr) {
               ui->Commandline->setText("!r " + getBaseStat(le) + " + " + le->text() + " + 1d10e1e10" + getModifiers());
               return;
           }
           ui->Commandline->setText("!r " + getBaseStat(le) + " + 1d10e1e10" + getModifiers());
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

void MainWindow::healthChanged()
{
    QObject *senderObj = sender(); // This will give Sender object
    QString senderObjName = senderObj->objectName();
    qDebug() << senderObjName;
    QList<QLineEdit*> list = getList();
    //QList<QLineEdit*> attributeList;

    QString attributeName = ("base_" + senderObjName);

    int value = 0;
    int result;

    foreach (QLineEdit* le, list) {
        if (le->objectName() == attributeName) {
            value = le->text().toInt();
            result = value - (-1*(ui->value_health->value() - 5));
            le->setText(QString::number(result));
        }
    }

    //value = ui->value_Int->text().toInt();
    //result = value - (-1*(ui->value_health->value() - 5));

    //ui->value_currentInt->setText(QString::number(result));
}

QString MainWindow::sendToBot()
{
    QString string = ui->Commandline->text();
    return string;
}
