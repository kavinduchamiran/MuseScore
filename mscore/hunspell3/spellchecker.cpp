#include "spellchecker.h"
#include "hunspell.hxx"

#if defined(near)
#undef near
#endif



SpellChecker::SpellChecker(const QString &dictionaryPath, const QString &userDictionary)
{
    new Hunspell("C:\en_US.aff", "C:\en_US.dic");
}

