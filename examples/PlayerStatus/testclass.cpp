#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  QString userSession, broadID;

  qDebug() << "input user session : ";
  qtin >> userSession;
  qDebug() << "input broadcast ID : ";
  qtin >> broadID;

  // create fetcher
  auto getter = new nicolive::PlayerStatus(this);

  // API classes emit "got" signal.
  // In this case, this lambda expression will receive.
  connect(getter, &nicolive::PlayerStatus::got, this,
          [](QString broadID, QString title, QString communityID, QString ownerID,
          QString ownerName, uint stTime, uint edTime, QString broadcastToken,
          QString  userID, bool isPremium, QString  addr, QString port, QString thread)
  {
    // unused variables
    (void)broadID, (void)communityID, (void)ownerID, (void)ownerName;
    (void)stTime, (void)edTime, (void)broadcastToken;
    (void)userID, (void)isPremium, (void)addr, (void)port, (void)thread;

    QTextStream out(stdout);
    out << title << endl;
    QCoreApplication::quit();
  });

  // if error occurred
  connect(getter, &nicolive::PlayerStatus::error, this, [](QString error){
    qDebug() << "error : " << error;
    QCoreApplication::quit();
  });
  // An error signal may be emitted more than once.

  // Finally, need to call get method.
  getter->get(broadID, userSession);
  
  // An instance of most API classes will be deleted automatically.
}
