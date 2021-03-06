﻿#include "userinfo.h"

namespace nicolive {

UserInfo::UserInfo(QString userID, QObject* parent) : HttpGetter(parent)
{
  this->userID = userID;
}

void UserInfo::get()
{
  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://api.ce.nicovideo.jp/api/v1/user.info?user_id=" + userID));

  requestGet(rq);
}

void UserInfo::gotReply(QNetworkReply* reply)
{
  StrAbstractor userinfo(QString(reply->readAll()));

  QString status = userinfo.midStr("status=\"", "\">");

  if (status != "ok") {
    const QString& code = userinfo.midStr("<code>", "</code>");
    const QString& description = userinfo.midStr("<description>", "</description>");
    emit error(code, description);
    return;
  }

  const QString& username = userinfo.midStr("<nickname>", "</nickname>");
  emit got(username);
}

}
