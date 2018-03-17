#ifndef LYRICSEDITOR_H
#define LYRICSEDITOR_H

#include "ui_lyricseditor.h"
#include "libmscore/musescoreCore.h"
#include "musescore.h"

#include "spellchecker.h"

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <Qtsql/QSqlQuery>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ms {

//---------------------------------------------------------
//   LyricsEditor
//---------------------------------------------------------

class LyricsEditor : public QDockWidget {
      Q_OBJECT

      QScrollArea* sa;
      Ui::LyricsEditor le;
      QSqlDatabase db;
      QNetworkAccessManager* nam = new QNetworkAccessManager(this);
      QStringList wordlist;

   public:
      LyricsEditor(MuseScore* parent);
      ~LyricsEditor();

      void setLyrics(QString lyrics)      { le.txtNormal->setText(lyrics); }
      QString getLyrics()                 { return le.txtRhythm->toPlainText(); }
      QString getSelectedText()           { return le.txtRhythm->textCursor().selectedText(); }
      void connectToDictionary();
      QString hyphenate(QString word);
      void getBingServer(QString word);

public slots:
      void setNormal();
      void setRhythm();
      void batchHyphenate();
      void checkSpellings();
      void responseReceived(QNetworkReply *reply);
      void customTextEditMenu(QPoint pos);
};

} // namespace Ms
#endif // LYRICSEDITOR_H
