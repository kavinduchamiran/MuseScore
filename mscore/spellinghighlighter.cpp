#include "spellinghighlighter.h"

void SpellingHighlighter::highlightBlock(const QString &text)
{
    if(text.isNull())
      return;

    QTextCharFormat badSpellingFmt;
    badSpellingFmt.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    badSpellingFmt.setUnderlineColor(Qt::red);

    QRegularExpression expression("\\w+"); // find all words

    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while (i.hasNext())
    {

        QRegularExpressionMatch match = i.next();
        QString word = match.captured(0);

        if(wordlist.contains(word)){
            qDebug() << word + " " + match.capturedStart();
          setFormat(match.capturedStart(), match.capturedLength(), badSpellingFmt);
          }
    }
}
