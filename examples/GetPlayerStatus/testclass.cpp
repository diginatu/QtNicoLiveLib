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

  // create fetcher and give user number that you want to know the name
  auto getter = new nicolive::GetPlayerStatus(broadID, userSession, this);

  // API classes emit "got" signal.
  // In this case, this lambda expression will receive.
  connect(getter, &nicolive::GetPlayerStatus::got, this,
          [](QString broadID, QString title, QString communityID, QString ownerName,
          uint stTime, uint edTime, QString broadcastToken)
  {
    // unused variables
    (void)broadID, (void)communityID, (void)ownerName;
    (void)stTime, (void)edTime, (void)broadcastToken;

    QTextStream out(stdout);
    out << title << endl;
    QCoreApplication::quit();
  });

  // if error occurred
  connect(getter, &nicolive::GetPlayerStatus::error, this, [](QString error){
    qDebug() << "error : " << error;
    QCoreApplication::quit();
  });
  // An error signal may be emitted more than once.

  // Finally, need to call get method.
  getter->get();
  
  // An instance of most API classes will be deleted automatically.
}
