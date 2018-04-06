#include <QtTest/QtTest>
#include "mtest/testutils.h"
#include "mscore/lyricseditor.h"

#define DIR QString("mscore/lyricseditor/")

using namespace Ms;

class TestLyricsEditor : public QObject, public MTest
      {
      Q_OBJECT

   private slots:
      void initTestCase();
      void testHyphenate();
      void testBingServer();
      };

//---------------------------------------------------------
//   initTestCase
//---------------------------------------------------------

void TestLyricsEditor::initTestCase()
      {
      initMTest();
      }

void TestLyricsEditor::testHyphenate()
{
  LyricsEditor* le = new LyricsEditor();

  QString referenceWord = "Abbe-vill-i-an";     //this is what the function should return
  QString sourceWord = "Abbevillian";           //input to the function

  QVERIFY(le->hyphenate(sourceWord) == referenceWord);    //checking whether function's output == reference
}

void TestLyricsEditor::testBingServer()
{
  LyricsEditor* le = new LyricsEditor();

  QString referenceWord = "school";     //this is what the function should return
  QString sourceWord = "schoool";           //input to the function

  QVERIFY(le->getBingServer(sourceWord) == referenceWord);    //checking whether function's output == reference
}

QTEST_MAIN(TestLyricsEditor)
#include "tst_lyricseditor.moc"

