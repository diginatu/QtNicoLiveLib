﻿#ifndef FETCHUSERNAME_H
#define FETCHUSERNAME_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

class FetchUserName : public HttpGetter
{
  Q_OBJECT
public:
  explicit FetchUserName(QString userID, QObject* parent = 0);
  ~FetchUserName();

  void get();

private:
  QString userID;
private slots:
  void gotReply(QNetworkReply* reply);

signals:
  void error();
  void got(QString username);

};

}

#endif // FETCHUSERNAME_H