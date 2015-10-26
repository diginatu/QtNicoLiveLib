#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  QString userNum;

  qDebug() << "input user ID : ";
  qtin >> userNum;

  // create fetcher and tell it user number that you want to know the name

  nicolive::FetchUserName* nameFetcher = new nicolive::FetchUserName(this, userNum);

  // QtNicoLiveLib classes emit "got" signal.
  // In this case, this lambda expression will receive.
  connect(nameFetcher, &nicolive::FetchUserName::got, this, [](QString name){
    QTextStream out(stdout);
    out << name << endl;
    QCoreApplication::quit();
  });

  // Finally, need to call get method.
  nameFetcher->get();
}
