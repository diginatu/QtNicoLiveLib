#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>
#include <fetchusername.h>
#include <QCoreApplication>
#include <iostream>

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
