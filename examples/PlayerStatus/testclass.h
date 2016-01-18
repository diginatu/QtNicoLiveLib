#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>
#include <QCoreApplication>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include <nl/playerstatus.h>

class TestClass : public QObject
{
  Q_OBJECT
public:
  explicit TestClass(QObject *parent = 0);
  void fetch();
};

#endif // TESTCLASS_H
