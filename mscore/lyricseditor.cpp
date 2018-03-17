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
      connect(le.txtRhythm, SIGNAL(textChanged()), parent, SLOT(updateLyricsScore()));

      connect(le.radNormal, SIGNAL(clicked()), this, SLOT(setNormal()));
      connect(le.radRhythm, SIGNAL(clicked()), this, SLOT(setRhythm()));

      connect(le.btnHyphenate, SIGNAL(clicked()), this, SLOT(batchHyphenate()));
      connect(le.btnCheckSpell, SIGNAL(clicked()), this, SLOT(checkSpellings()));

      connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(responseReceived(QNetworkReply*)));
      connect(le.txtNormal, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customTextEditMenu(QPoint)));

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

void Ms::LyricsEditor::batchHyphenate(){
    if(le.radNormal->isChecked()){
        QString lyrics = le.txtNormal->toPlainText();
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
    getBingServer("schoool");
}

void Ms::LyricsEditor::customTextEditMenu(QPoint pos)
{
    /* Create an object context menu */
    QMenu* menu = new QMenu();
    /* Create actions to the context menu */

//    QTextCursor tc1 =
//    int i = tc1.position();
    QTextCursor tc = le.txtNormal->cursorForPosition(pos);
    tc.select(QTextCursor::WordUnderCursor);

    qDebug() << tc.selectedText();
    getBingServer(tc.selectedText());


//    QAction * editDevice = new QAction(trUtf8("kkk"), this);
//    QAction * deleteDevice = new QAction(trUtf8("jjj"), this);

    /* Connect slot handlers for Action pop-up menu */
//    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Call Handler dialog editing
//    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Handler delete records
//    connect(ui.saveAction, SIGNAL(triggered()), this, SLOT(saveNew()));
    /* Set the actions to the menu */

//    menu->addAction(editDevice);
//    menu->addAction(deleteDevice);

    for(QString word: wordlist)
        menu->addAction(new QAction(word, this));

    menu->addSeparator();

    foreach (QAction *action, le.txtNormal->createStandardContextMenu()->actions()) {
        menu->addAction(action);
    }

    /* Call the context menu */
    menu->popup(le.txtNormal->viewport()->mapToGlobal(pos));
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

    qDebug() << word;

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

    request.setRawHeader("Ocp-Apim-Subscription-Key", "1c228399f957411ba733747b604a2cad");

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
//                return jsonObject["flaggedTokens"].toArray();


    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
//                    propertyNames.append(obj["PropertyName"].toString());
//                    propertyKeys.append(obj["key"].toString());
//        qDebug() << obj["token"].toString();

        QJsonArray jsonArray2 = obj["suggestions"].toArray();
        foreach (const QJsonValue & value2, jsonArray2) {
            QJsonObject obj2 = value2.toObject();
//            qDebug() << obj2["suggestion"].toString();
            wordlist.append(obj2["suggestion"].toString());
//            qDebug() << QString::number(obj2["score"].toDouble());
        }

    }

    qDebug() << wordlist;
}
