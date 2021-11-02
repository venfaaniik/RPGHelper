#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>

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
    //connect(ui->value_health, SIGNAL(valueChanged(int)), this, SLOT(healthChanged()));
    connect(ui->ResetLuck, SIGNAL(clicked()), this, SLOT(resetLuck()));
    connect(ui->dice_d10, SIGNAL(clicked()), this, SLOT(basicDice()));
    connect(ui->dice_d20, SIGNAL(clicked()), this, SLOT(basicDice()));
    connect(ui->dice_d50, SIGNAL(clicked()), this, SLOT(basicDice()));
    connect(ui->dice_d100, SIGNAL(clicked()), this, SLOT(basicDice()));

    //connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveTo()));
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
    //qDebug() << parent;

    //holyfuck jos jollain on parempi tapa tähä nii voi jakaa
    //a little if-mess, as a treat
    if (parent == "Group_INTELLIGENCE"){
        return value = ui->value_Int->text();
    } else if (parent == "Group_REFLEX"){
        return value = ui->value_Reflex->text();
    } else if (parent == "Group_DEXTERITY"){
        return value = ui->value_Dexterity->text();
    } else if (parent == "Group_BODY"){
        return value = ui->value_Body->text();
    } else if (parent == "Group_EMPATHY"){
        return value = ui->value_Empathy->text();
    } else if (parent == "Group_CRAFTING"){
        return value = ui->value_Crafting->text();
    } else if (parent == "Group_WILL"){
        return value = ui->value_Will->text();
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
    QString attributeName = ("value_" + senderObjName);
    //QString dice = ("dice_" + senderObjName);
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
    //TECHNICALLY WE DO NOT NEED THIS AS ALL THE MINUSES ARE ALREADY INCLUDED IN THE ROLL WHEN HEALTH CHANGES
    //TODO ONLY IF GOT EXTRA TIME.
    int value = 0;
    int result;
    QList<QLineEdit*> list = getList();

    foreach (QLineEdit* le, list) {
        QString parent = le->parentWidget()->objectName();
        if (parent == "currentStats") {
            //value = le->text().toInt();
            qDebug() << le->objectName();
            value = 0;
            result = value - (-1*(ui->value_health->value() - 5));
            //qDebug() << le->objectName() << " + " << le->text();
            le->setText(QString::number(result));
        }
    }

    //value = ui->value_Int->text().toInt();
    //result = value - (-1*(ui->value_health->value() - 5));

    //ui->value_currentInt->setText(QString::number(result));
}

void MainWindow::resetLuck()
{
    int luck = ui->value_currentLuck->text().toInt();
    ui->value_luck->setValue(luck);
}

void MainWindow::saveTo()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Character Sheet"), "",
                                                    tr("Character Sheet (*.txt);;All Files(*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file."), file.errorString());
            return;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out << savingValues;
    }

}

void MainWindow::loadFrom()
{

}

void MainWindow::basicDice()
{
    QString dice = ((QPushButton*)sender())->text();
    ui->Commandline->setText("!r " + dice);
    //ui->Commandline->setText("!r " + " + 1d10e1e10");
}

QString MainWindow::sendToBot()
{
    QString string = ui->Commandline->text();
    return string;
}
