#include "spellchecker.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#include "hunspell/1.6.2/src/hunspell/hunspell.hxx"

SpellChecker::SpellChecker(const QString &dictionaryPath, const QString &userDictionary)
{
  Hunspell * h = new Hunspell("A", "B");
}


SpellChecker::~SpellChecker()
{

}


bool SpellChecker::spell(const QString &word)
{

}


QStringList SpellChecker::suggest(const QString &word)
{

}


void SpellChecker::ignoreWord(const QString &word)
{

}


void SpellChecker::put_word(const QString &word)
{

}


void SpellChecker::addToUserWordlist(const QString &word)
{

}
