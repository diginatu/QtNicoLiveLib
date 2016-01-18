#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include "testclass.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  // this Library can be used in a QObject class.
  TestClass* test = new TestClass;
  test->fetch();

  return a.exec();
}
