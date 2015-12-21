#include "publishstatus.h"

namespace nicolive {

PublishStatus::PublishStatus(QObject* parent) :
  HttpGetter(parent)
{

}

void PublishStatus::get(const QString& broadID, const QString& userSession)
{
  // make reques
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getpublishstatus?v=" + broadID));

  requestGet(rq);
}

void PublishStatus::gotReply(QNetworkReply* reply)
{
  StrAbstractor heartbeat_data(QString(reply->readAll()));

  const QString status = heartbeat_data.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    const QString& code = heartbeat_data.midStr("<code>","</code>");
    emit error(code);
    return;
  }

  const QString& token = heartbeat_data.midStr("<token>","</token>");
  emit got(token);
}

}
