#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

class Hunspell;

class SpellChecker
{
public:
    SpellChecker(const QString &dictionaryPath, const QString &userDictionary);
    ~SpellChecker();

    bool spell(const QString &word);
    QStringList suggest(const QString &word);
    void ignoreWord(const QString &word);
    void addToUserWordlist(const QString &word);

private:
    void put_word(const QString &word);
    Hunspell *_hunspell;
    QString _userDictionary;
};

#endif // SPELLCHECKER_H
