#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  String userSession, broadID, text;

  qDebug() << "input user session : ";
  qtin >> userSession;
  qDebug() << "input broadcast ID : ";
  qtin >> broadID;
  qDebug() << "input owner comment : ";
  qtin >> text;

  // create fetcher
  auto ft = new nicolive::PublishStatus(this);

  // API classes emit "got" signal.
  // In this case, this lambda expression will receive.
  connect(ft, &nicolive::PublishStatus::got, this,
          [=](QString token) {
    qDebug() << broadID;
    auto cmft = new nicolive::OwnerComment(this);
    // OwnerComment class also emit "got" signal.
    connect(cmft, &nicolive::OwnerComment::got, this, [](){
      qDebug() << "submitted";
    });
    // if error occurred
    connect(cmft, &nicolive::OwnerComment::error, this, [](QString code){
      qDebug() << code;
      QCoreApplication::quit();
    });
    cmft->get(broadID, text, "", token, userSession);
  });

  // if error occurred
  connect(ft, &nicolive::PublishStatus::error, this, [=](QString error){
    qDebug() << "error : " << error;
    QCoreApplication::quit();
  });

  // Finally, need to call get method.
  ft->get(broadID, userSession);
  
  // An instance of most API classes will be deleted automatically.
}
