#include "commpostkey.h"

namespace nicolive {

CommPostKey::CommPostKey(QString thread, int blockNo, const QString& userSession, QObject *parent) :
  HttpGetter(parent)
{
  this->thread = thread;
  this->blockNo = blockNo;
  this->userSession = userSession;
}

void CommPostKey::get()
{
  // make request
  QNetworkRequest rq;
  QVariant&& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl(QString("http://live.nicovideo.jp/api/getpostkey?") +
             "thread=" + thread +
             "&block_no=" + QString::number(blockNo)));

  requestGet(rq);
}

void CommPostKey::gotReply(QNetworkReply* reply)
{
  const auto postKey = QString(reply->readAll().mid(8));
  if(postKey.isEmpty()) {
    emit error();
    return;
  }
  emit got(postKey);
}

}

