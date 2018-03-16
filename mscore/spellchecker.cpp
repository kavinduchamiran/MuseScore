#include "spellchecker.h"
#include "ui_spellchecker.h"

SpellChecker::SpellChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpellChecker)
{
    ui->setupUi(this);
}

SpellChecker::~SpellChecker()
{
    delete ui;
}
