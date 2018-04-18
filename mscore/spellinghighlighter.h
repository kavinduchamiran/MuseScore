#ifndef SPELLINGHIGHLIGHTER_H
#define SPELLINGHIGHLIGHTER_H

#include <QObject>



#include <QSyntaxHighlighter>

class SpellingHighlighter : public QSyntaxHighlighter
{
private:
    QMap<QString, QStringList> wordlist;

public:
    void setWordlist(QMap<QString, QStringList> list) { wordlist = list; }
    SpellingHighlighter(QObject* parent) : QSyntaxHighlighter(parent) {}
    void highlightBlock(const QString &text);
};

#endif // SPELLINGHIGHLIGHTER_H
