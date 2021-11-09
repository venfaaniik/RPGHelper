#include "grimoiretest.h"
#include "ui_grimoiretest.h"

Grimoiretest::Grimoiretest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Grimoiretest)
{
    ui->setupUi(this);

    QTableWidget *tableWidget = ui->spellTable;
}

Grimoiretest::~Grimoiretest()
{
    delete ui;
}

void Grimoiretest::addToList()
{

    //ui->spellTable->setItem()

    //QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg( (row+1)*(column+1)));
    //ui->spelltable->setItem(row, column, newItem);
}
