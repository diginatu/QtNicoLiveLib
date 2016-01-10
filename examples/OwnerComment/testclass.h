#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>
#include <QCoreApplication>
#include <iostream>
#include <nl/livewaku.h>
#include <nl/ownercomment.h>

class TestClass : public QObject
{
  Q_OBJECT
public:
  explicit TestClass(QObject *parent = 0);
  void fetch();

private:
  QString userSession, broadID, text;
  nicolive::LiveWaku* waku;
signals:

public slots:
};

#endif // TESTCLASS_H
