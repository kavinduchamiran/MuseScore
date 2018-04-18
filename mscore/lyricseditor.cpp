#include "lyricseditor.h"
#include "spellchecker.h"

namespace Ms {

//---------------------------------------------------------
//   showLyricsEditor
//---------------------------------------------------------

void MuseScore::showLyricsEditor(bool visible)
      {
      QAction* a = getAction("toggle-lyricseditor");
      if (!_lyricsEditor) {
            _lyricsEditor = new LyricsEditor(this);
            connect(_lyricsEditor, SIGNAL(visibilityChanged(bool)), a, SLOT(setChecked(bool)));

            addDockWidget(Qt::RightDockWidgetArea, _lyricsEditor);
            }
      if (visible) {
            updateLyricsEditor();
            }
      if (_lyricsEditor)
            _lyricsEditor->setVisible(visible);
      }

//---------------------------------------------------------
//   LyricsEditor
//---------------------------------------------------------

LyricsEditor::LyricsEditor(MuseScore* parent)
   : QDockWidget(tr("Lyrics Editor"), parent)
      {
      setObjectName("LyricsEditor");
      setAllowedAreas(Qt::DockWidgetAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea));
      sa = new QScrollArea;
      sa->setAccessibleName(tr("Lyrics Editor Subwindow"));
      sa->setFrameShape(QFrame::NoFrame);
      sa->setWidgetResizable(true);
      setWidget(sa);
      sa->setFocusPolicy(Qt::NoFocus);

      le.setupUi(sa);

      // connecting editor to updateLyricsScore()
      connect(le.txtRhythm, SIGNAL(textChanged()), this, SLOT(updateee()));//updateLyricsScore

      connect(le.radNormal, SIGNAL(clicked()), this, SLOT(setNormal()));
      connect(le.radRhythm, SIGNAL(clicked()), this, SLOT(setRhythm()));

      connect(le.btnHyphenate, SIGNAL(clicked()), this, SLOT(batchHyphenate()));
      connect(le.btnCheckSpell, SIGNAL(clicked()), this, SLOT(checkSpellings()));

      connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(responseReceived(QNetworkReply*)));
      connect(le.txtRhythm, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customTextEditMenu(QPoint)));

      highlighter = new SpellingHighlighter(le.txtRhythm);
      connectToDictionary();        //loading dictionary
      }

LyricsEditor::~LyricsEditor()
      {
//      disconnect(le.textEdit, SIGNAL(textChanged()), kms, SLOT(updateLyricsScore()));
      }

}

void Ms::LyricsEditor::setNormal(){
    le.lblMode->setText("Normal Mode");
    le.txtRhythm->hide();
    le.txtNormal->show();
}

void Ms::LyricsEditor::setRhythm(){
    le.lblMode->setText("Rhythm Mode");
    le.txtNormal->hide();
    le.txtRhythm->show();
}

void Ms::LyricsEditor::replaceWord(){
  QAction *act = qobject_cast<QAction *>(sender());
  QVariant v = act->data();
  QString replacement = act->text();
  QPoint pos = v.toPoint();
  QTextCursor cursor = le.txtRhythm->cursorForPosition(pos);   // replacement logic
  cursor.select(QTextCursor::WordUnderCursor);
  cursor.removeSelectedText();
  cursor.insertText(replacement);
}

void Ms::LyricsEditor::updateee(){
  qDebug() << "called";
  highlighter->setWordlist(wordlist);
  highlighter->highlightBlock(le.txtRhythm->toPlainText());
}

void Ms::LyricsEditor::batchHyphenate(){
    if(le.radNormal->isChecked()){
        QString lyrics = le.txtRhythm->toPlainText();
        QString finalStr;

        QStringList str  = lyrics.split(QRegExp("\n"));

        for(QString s: str){
            if(!s.trimmed().isNull()){
                QStringList words = s.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                for(QString word: words){
                    finalStr += (hyphenate(word) + " ");
                }
                finalStr = finalStr.trimmed() + "\n";
            }
        }

        le.txtRhythm->setText(finalStr);
        le.radRhythm->animateClick();
    }else{
        QTextCursor cursor = le.txtRhythm->textCursor();
        QString finalStr;
        if(cursor.hasSelection())
        {
            QStringList words = getSelectedText().split(QRegExp("\\s+"), QString::SkipEmptyParts);
            // Aaron Aaronitic Aachen
            for(QString word: words){
                finalStr += (hyphenate(word) + " ");
            }
        }
        cursor.insertText(finalStr.trimmed());
    }
}

void Ms::LyricsEditor::checkSpellings(){
//    le.radNormal->animateClick();
    getBingServer(le.txtRhythm->toPlainText());
    highlighter->setWordlist(wordlist);
    highlighter->highlightBlock(le.txtRhythm->toPlainText());
    le.txtRhythm->setText(le.txtRhythm->toPlainText() + "");
}

void Ms::LyricsEditor::customTextEditMenu(QPoint pos)
{
    QMenu* menu = new QMenu();

    QTextCursor tc = le.txtRhythm->cursorForPosition(pos);
    tc.select(QTextCursor::WordUnderCursor);

    QString selectedWord = tc.selectedText();
    selectedWord.replace("-", "");
    selectedWord.replace("_", "");

    qDebug() << selectedWord;

    for(QString word: wordlist[selectedWord]){
        QAction* action = new QAction(word, this);
        action->setData(pos);
        connect(action, SIGNAL(triggered()), this, SLOT(replaceWord()));
        menu->addAction(action);
      }

    menu->addSeparator();

    foreach (QAction *action, le.txtRhythm->createStandardContextMenu()->actions()) {
        menu->addAction(action);
    }

    /* Call the context menu */
    menu->popup(le.txtRhythm->viewport()->mapToGlobal(pos));
}


void Ms::LyricsEditor::connectToDictionary(){
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("C:/Users/Kavindu/MuseScore/mscore/dictionary.sqlite");      //make this a relative path

    if(db.open()){
        qDebug("Connected to dictionary");
    }else{
        qDebug("Error connecting to dictionary");
    }
}


QString Ms::LyricsEditor::hyphenate(QString word){
    if(db.open()){
        QSqlQuery qry;

        qry.prepare("select hyphenation from dictionary where word like (:word)");

        qry.bindValue(":word", word);

        if(qry.exec()){
            while(qry.next()){
                QString hypword = qry.value("hyphenation").toString();  //need to preserve case sensitivity. add dashes to corresponsing positions

                if(hypword != ""){
                    return hypword;
                }else{
                    return word;
                }
            }
        }
    }
    return word;
}

void Ms::LyricsEditor::getBingServer(QString word){
    if(word.isNull())
        return;

    word.replace("\n", " ");

    QString mkt = "en-US";
    QString mode = "proof";
//    QString text = "Hollo, wrld!";

    QUrl url("https://api.cognitive.microsoft.com/bing/v7.0/spellcheck/");
    QUrlQuery query;

    query.addQueryItem("mkt", mkt);
    query.addQueryItem("mode", mode);
    query.addQueryItem("text", word);

    url.setQuery(query.query());

    QNetworkRequest request(url);

    request.setRawHeader("Ocp-Apim-Subscription-Key", "29d375a6ade243cea87a657ff045180a");

    nam->get(request);
}

void Ms::LyricsEditor::responseReceived(QNetworkReply *reply){
    wordlist.clear();
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QByteArray mDataBuffer = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(mDataBuffer);

    QJsonObject jsonObject = doc.object();
    QJsonArray jsonArray = jsonObject["flaggedTokens"].toArray();

    foreach (const QJsonValue & value, jsonArray) {
        QStringList suggestions;

        QJsonObject obj = value.toObject();
        QString word = obj["token"].toString();
        QJsonArray jsonArray2 = obj["suggestions"].toArray();
        foreach (const QJsonValue & value2, jsonArray2) {
            QJsonObject obj2 = value2.toObject();
            suggestions.append(obj2["suggestion"].toString());
        }
        wordlist[word] = suggestions;
    }

    highlighter->setWordlist(wordlist);
    qDebug() << wordlist;
}
