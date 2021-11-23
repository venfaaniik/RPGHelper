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

    grimoire = new Grimoiretest();

    //TODO: FIND OUT HOW TO IGNORE OTHER THAN SKILL BUTTONS HERE
    QList<QPushButton*> pbList = findChildren<QPushButton*> (QString(), Qt::FindChildrenRecursively);
    foreach (QPushButton* pb, pbList) {
        //couldn't just yeet the lineedit list as an argument to the onSKillCLicked because cliked() is also empty. idk what kind of stupidity is that.
        connect(pb, SIGNAL(clicked()), this, SLOT(onSkillClicked()));
        pb->setStyleSheet("Text-align:left");
    }

    //init all the tables in main screen
    initTables();

    connect(ui->CopyToClipBoard, SIGNAL(clicked()), this, SLOT(copyToClipboard()));
    //connect(ui->value_health, SIGNAL(valueChanged(int)), this, SLOT(healthChanged()));
    connect(ui->ResetLuck, SIGNAL(clicked()), this, SLOT(resetLuck()));
    connect(ui->dice_d10,  SIGNAL(clicked()), this, SLOT(basicDice()));
    connect(ui->dice_d20,  SIGNAL(clicked()), this, SLOT(basicDice()));
    connect(ui->dice_d50,  SIGNAL(clicked()), this, SLOT(basicDice()));
    connect(ui->dice_d100, SIGNAL(clicked()), this, SLOT(basicDice()));

    connect(ui->actionGrimoire, SIGNAL(triggered()), this, SLOT(showGrimoire()));


    //connect(ui->Table_Will, SIGNAL(cellClicked(int,int)), this, SLOT(onCellClicked(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTables()
{
    //this is horrible, this is terrible but it's ok for my needs...
    int verticalHeader = 112;

    QList<QTableWidget*> tableList = findChildren<QTableWidget*> (QString(), Qt::FindChildrenRecursively);
    foreach(QTableWidget* tb, tableList) {
      tb->verticalHeader()->setFixedWidth(verticalHeader);
      //tb->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignHCenter);

      //just DEBUG, tested centering because not all values are centered, that's one hell of a työmaa
      for (int i = 0; i < tb->rowCount(); ++i) {
           QTableWidgetItem *item = tb->item(i,0);
           if(!item) {
               item= new QTableWidgetItem();
               tb->setItem(i,0,item);
           } else { //honestly, l didn't expect this to work.
               //to center all the pre-existing table items, could've done this in the editor but l'm lazy
               QTableWidgetItem *preset = tb->itemAt(i,0);
               preset->setTextAlignment(Qt::AlignHCenter);
           }
           item->setText("");
           item->setTextAlignment(Qt::AlignHCenter);
      }
      ui->retranslateUi(this);

      // lähetääs lambdailee, täl saa clicked name etc...
      // can be used to get clicked events.
      auto header = tb->verticalHeader();
      //QString s = getSkill();

      connect(header, &QHeaderView::sectionClicked, [tb](int index) {
          QString text = tb->verticalHeaderItem(index)->text();
          qDebug() << index << " text: " << text << tb->item(index,0)->text();
          auto getText = [](QTableWidget *tb, int index) -> QString {
              QString text = tb->item(index,0)->text();
              return text;
          };
          QString s = getText(tb, index);
          qDebug() << s;
      });

      //actual connect l want to use
      //connect(tb->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onCellClicked(int)));
    }
}

void MainWindow::setSkill(QString s)
{
    skill = &s;
}

QString MainWindow::getSkill()
{
    return *skill;
}



QList<QLineEdit *> MainWindow::getList()
{
    // Just to get the list of buttons kek
    QList<QLineEdit*> leList = findChildren<QLineEdit*> (QString(), Qt::FindChildrenRecursively);
    return leList;
}

QString MainWindow::getBaseStat(QLineEdit* le)
{
    QString value;
    QString parent = le->parentWidget()->objectName(); // get the parent-object name for checks

    //holyfuck jos jollain on parempi tapa tähä nii voi jakaa
    //a little if-mess, as a treat
    /*if (parent == "Group_INTELLIGENCE"){
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
    }*/
    return "error";
}

QString MainWindow::getModifiers()
{
    QString value;

    //POSITIVES
    // if any other unnamed buffs are to be applied
    if (ui->Other_Buffs->value() > 0) {
        qDebug() << "Other Buffs: " << ui->Other_Buffs->value();
        value = " + " + QString::number(ui->Other_Buffs->value());
    }

    // if the player if well fed AND/OR well rested
    if (ui->WeddFed->isChecked() && ui->WellRested->isChecked() == true) {
        value = value + " + 2 [WellFed | WellRested]";
    } else if (ui->WeddFed->isChecked() == true) {
        value = value + " + 1 [WellFed]";
    } else if (ui->WellRested->isChecked() == true) {
        value = value + " + 1 [WellRested]";
    }

    //NEGATIVES
    // if any other negatives are to be applied
    if (ui->Other_Buffs->value() < 0) {
        qDebug() << "Other Buffs: " << ui->Other_Buffs->value();
        value = value + " - " + QString::number(-1 * ui->Other_Buffs->value());
    }

    // if the player is damaged and wounded-penalty is to be applied
    if (ui->value_health->value() < 5) {
        int helth = -1 *(ui->value_health->value() - 5);
        value = value + " - " + QString::number(helth);
    }
    return value;
}

//a multitasker.
void MainWindow::onSkillClicked()
{  
    //Deprecated
    /*//Get sender-object for further operations
    QObject *senderObj = sender();
    QString senderObjName = senderObj->objectName();

    // we'll assume it's both skill and attribute and later check if such thing exists
    QString skillName = (senderObjName + "_Value");
    QString attributeName = ("value_" + senderObjName);

    // get the list to go through
    QList<QLineEdit*> list = getList();
    foreach (QLineEdit* le, list) {
       if (le->objectName() == attributeName) { // if it's an attribute
            ui->Commandline->setText("!r " + le->text() + " + 1d10e1e10" + getModifiers());
            return;

       } else if (le->objectName() == skillName) { // if it's a skill
           if (le->text() != nullptr) { // if skill has any points allocated
               ui->Commandline->setText("!r " + getBaseStat(le) + " + " + le->text() + " + 1d10e1e10" + getModifiers());
               return;
           }
           ui->Commandline->setText("!r " + getBaseStat(le) + " + 1d10e1e10" + getModifiers());
           return;
       }
    }*/

    qDebug() << "boop.";
}

void MainWindow::onCellClicked(int index)
{
    qDebug() << index << "boop.";
}

// copy the whole string to clipboard to paste it on Discord.
void MainWindow::copyToClipboard()
{
    QString string = ui->Commandline->text();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(string);

    // an artefact from a time I attempted throwing it over network
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
    //resets luck to the current luck value
    //because l never remember to do this in the session start
    //int luck = ui->value_currentLuck->text().toInt();
    //ui->value_luck->setValue(luck);
}


//UHHHH
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

// UHHHHHHHHH
void MainWindow::loadFrom()
{

}

void MainWindow::basicDice()
{
    //basic dice to throw, d10, d20, d50, d100
    QString dice = ((QPushButton*)sender())->text();
    ui->Commandline->setText("!r " + dice);
}

void MainWindow::changeWindow()
{
    //open grimoire window
    if(grimoire->isVisible()) {
        grimoire->hide();
        this->show();
    } else {
        this->hide();
        grimoire->show();
    }
}

void MainWindow::showGrimoire()
{
    //aaand show it
    grimoire->show();
}


QString MainWindow::sendToBot()
{
    //old stuff, it works but discord side didn't want to work.
    //(bot didn't anwser to bot messages)
    QString string = ui->Commandline->text();
    return string;
}
