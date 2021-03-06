﻿#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  QString userNum;

  qDebug() << "input user ID : ";
  qtin >> userNum;

  // create fetcher and give user number that you want to know the name
  nicolive::UserInfo* nameFetcher = new nicolive::UserInfo(userNum, this);

  // API classes emit "got" signal.
  // In this case, this lambda expression will receive.
  connect(nameFetcher, &nicolive::UserInfo::got, this, [](QString name){
    QTextStream out(stdout);
    out << name << endl;
    QCoreApplication::quit();
  });

  // if error occurred
  connect(nameFetcher, &nicolive::UserInfo::error, this,
          [](QString code, QString description){
    qDebug() << "error " << code << " " << description;
    QCoreApplication::quit();
  });
  // An error signal may be emitted more than once.

  // Finally, need to call get method.
  nameFetcher->get();

  // An instance of most API classes will be deleted automatically.
}
