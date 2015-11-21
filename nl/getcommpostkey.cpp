#include "getcommpostkey.h"

namespace nicolive {

GetCommPostKey::GetCommPostKey(QString thread, int blockNo, const QString& userSession, QObject *parent) :
  HttpGetter(parent)
{
  this->thread = thread;
  this->blockNo = blockNo;
  this->userSession = userSession;
}

void GetCommPostKey::get()
{
  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl(QString("http://live.nicovideo.jp/api/getpostkey?") +
             "thread=" + thread +
             "&block_no=" + QString::number(blockNo)));

  requestGet(rq);
}

void GetCommPostKey::gotReply(QNetworkReply* reply)
{
  auto postKey = reply->readAll().mid(8);

  emit got(QString(postKey));

  reply->deleteLater();
  this->deleteLater();
}

}

