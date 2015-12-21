#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>
#include <QCoreApplication>
#include <iostream>
#include <nl/playerstatus.h>

class TestClass : public QObject
{
  Q_OBJECT
public:
  explicit TestClass(QObject *parent = 0);
  void fetch();

signals:

public slots:
};

#endif // TESTCLASS_H
