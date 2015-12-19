#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  QString userSession;

  qDebug() << "input user session : ";
  qtin >> userSession;

  // create fetcher and pass usersession
  auto ft = new nicolive::FetchFavoritedNowWaku(this);
  // API classes emit "got" signal.
  // In this case, this lambda expression will receive.
  // in this class, got signal may be called more than once
  connect(ft, &nicolive::FetchFavoritedNowWaku::got, this, [](nicolive::LiveWaku* mywaku){
    qDebug() << "got a waku : " << mywaku->getBroadID() <<
                " in " << mywaku->getCommunityID();
    // mywaku should be deleted when it is not used.
    mywaku->deleteLater();
  });
  // if error occurred
  connect(ft, &nicolive::FetchFavoritedNowWaku::error, this, [](QString err){
    qDebug() << "FetchFavoritedNowWaku error : " << err;
  });
  // if destroyed (it means all favorited wakus was already emitted)
  connect(ft, &nicolive::FetchFavoritedNowWaku::destroyed, this, [](){
    QCoreApplication::quit();
  });
  // Finally, need to call get method.
  ft->get(userSession);
  
  // An instance of most API classes will be deleted automatically.
}
