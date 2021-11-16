#include "grimoiretest.h"
#include "ui_grimoiretest.h"

#include <QtCore>

Grimoiretest::Grimoiretest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Grimoiretest)
{
    ui->setupUi(this);

    spellTable = ui->spellTable;

    connect(ui->plus, SIGNAL(clicked()), this, SLOT(addToTable()));
    connect(ui->minus, SIGNAL(clicked()), this, SLOT(removeFromTable()));
    connect(ui->sort, SIGNAL(clicked()), this, SLOT(sort()));

}

Grimoiretest::~Grimoiretest()
{
    delete ui;
}

void Grimoiretest::addToTable()
{
    int row = spellTable->rowCount();
    spellTable->insertRow(row);
}

void Grimoiretest::removeFromTable()
{
    //removes all selected items from the table
     const QList<QTableWidgetItem *> selected = spellTable->selectedItems();
     for (QTableWidgetItem *item : selected) {
         spellTable->removeRow(item->row());
     }
}

void Grimoiretest::sort()
{
    spellTable->sortItems(0,Qt::AscendingOrder);
}
