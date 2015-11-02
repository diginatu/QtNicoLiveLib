#include "fetchusername.h"

namespace nicolive {

FetchUserName::FetchUserName(QString userID, QObject* parent) :
  HttpGetter(parent)
{
  this->userID = userID;
}

FetchUserName::~FetchUserName()
{
}

void FetchUserName::get()
{
  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://api.ce.nicovideo.jp/api/v1/user.info?user_id=" + userID));

  requestGet(rq);
}

void FetchUserName::gotReply(QNetworkReply* reply)
{
  StrAbstractor userinfo(QString(reply->readAll()));

  QString status = userinfo.midStr("status=\"", "\">");

  if (status == "fail") {
    emit error();
    reply->deleteLater();
    this->deleteLater();
    return;
  }

  QString username = userinfo.midStr("<nickname>", "</nickname>");
  emit got(username);

  reply->deleteLater();
  this->deleteLater();
}

}
