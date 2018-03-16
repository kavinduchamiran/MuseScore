#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include "lyricseditor.h"


#include <QDialog>

namespace Ui {
class SpellChecker;
}

class SpellChecker : public QDialog
{
    Q_OBJECT

public:
    explicit SpellChecker(QWidget *parent = 0);
    ~SpellChecker();

private:
    Ui::SpellChecker *ui;
};

#endif // SPELLCHECKER_H
