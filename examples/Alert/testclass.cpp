#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  QString mail, pass;

  qDebug() << "input user mail : ";
  qtin >> mail;
  qDebug() << "input user password : ";
  qtin >> pass;

  qDebug() << mail << pass;

  // create LiveWaku, which deals about alert
  auto login = new nicolive::AlertLogin(this);
  auto admin = new nicolive::AlertAdmin(this);
  nicolive::AlertConnection* alertConnection;

  connect(login, &nicolive::AlertLogin::error,
          this, [](QString code, QString description){
    qDebug() << "login error : " << code << "\n" << description;
    QCoreApplication::quit();
  });
  connect(login, &nicolive::AlertLogin::got, this, [=](QString ticket){
    admin->get(ticket);
  });

  connect(admin, &nicolive::AlertAdmin::error, this, [](QString code){
    qDebug() << "admin error : " << code;
    QCoreApplication::quit();
  });
  connect(admin, &nicolive::AlertAdmin::got,
          this, [&](QStringList myCommunities, QString addr,
          qint16 port, QString thread){
    qDebug() << "your communities : " << myCommunities << "\n";

    alertConnection = new nicolive::AlertConnection(addr, port, thread, this);
    connect(alertConnection, &nicolive::AlertConnection::error,
            this, [](QString errorPosition, QString code){
      qDebug() << "alert error " << errorPosition << " : " << code;
      QCoreApplication::quit();
    });
    connect(alertConnection, &nicolive::AlertConnection::gotNewWaku,
            this, [](QString broadID, QString communityID, QString nushiID){
      (void)communityID, (void)nushiID;
      qDebug() << "broadID : " << broadID;
    });
    alertConnection->doConnect();
  });

  login->get(mail, pass);
}
