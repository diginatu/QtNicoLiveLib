#include "heartbeat.h"

namespace nicolive {

HeartBeat::HeartBeat(QObject *parent) : HttpGetter(parent)
{

}

void HeartBeat::get(const QString& broadID, const QString& userSession)
{
  // make reques
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/heartbeat?v=" + broadID));

  requestGet(rq);
}

void HeartBeat::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor heartbeat_data(QString(reply->readAll()));

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    QString code(heartbeat_data.midStr("<code>","</code>"));
    QString description(heartbeat_data.midStr("<description>","</description>"));

    emit error(code, description);
    return;
  }

  QString watchCount(heartbeat_data.midStr("<watchCount>","</watchCount>"));
  QString commentCount(heartbeat_data.midStr("<commentCount>","</commentCount>"));

  emit got(watchCount, commentCount);
}

}
