#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);

  qDebug() << "input user session : ";
  qtin >> userSession;
  qDebug() << "input broadcast ID : ";
  qtin >> broadID;
  qDebug() << "input owner comment : ";
  qtin >> text;


  // create a broadcast manager
  // This instance will be deleted with this class.
  waku = new nicolive::LiveWaku(broadID, this);

  // API classes emit signals.
  // In this case, this lambda expression will receive.
  connect(waku, &nicolive::LiveWaku::gotOwnerCommentToken, this, [&](){
    auto cmft = new nicolive::OwnerComment(this);
    // OwnerComment class emits "got" signal if it correctly do.
    connect(cmft, &nicolive::OwnerComment::got, this, [](){
      qDebug() << "submitted";
      QCoreApplication::quit();
    });
    // if error occurred
    connect(cmft, &nicolive::OwnerComment::error, this, [](QString code){
      qDebug() << code;
      QCoreApplication::quit();
    });
    cmft->get(text, "", *waku, userSession);
  });

  // if error occurred
  connect(waku, &nicolive::LiveWaku::gotOwnerCommentTokenError,
          this, [](){
    qDebug() << "fetchOwnerCommentToken error";
    QCoreApplication::quit();
  });

  // Finally, need to call get method.
  waku->fetchOwnerCommentToken(userSession);
  
  // An instance of most API classes will be deleted automatically.
}
