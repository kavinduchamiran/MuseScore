#include "spellchecker.h"
#include "ui_spellchecker.h"

SpellChecker::SpellChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpellChecker)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));



    QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
    model->setItem(0,0,firstRow);

    ui->tblWords->setModel(model);
}

SpellChecker::~SpellChecker()
{
    delete ui;
}
