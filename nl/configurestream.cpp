#include "configurestream.h"

namespace nicolive {

ConfigureStream::ConfigureStream(QObject *parent) : HttpGetter(parent)
{

}

void ConfigureStream::get(const QString& key, const QString& value,
                          const LiveWaku& waku, const QString userSession)
{
  // make reques
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl(QString("http://watch.live.nicovideo.jp/api/configurestream/"
                         "lv%1?key=%2&version=2&value=%3&token=%4")
                  .arg(waku.getBroadID()).arg(key).arg(value)
                  .arg(waku.getBroadcastToken())));

  requestGet(rq);
}

void ConfigureStream::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor rep(QString(reply->readAll()));

  const QString& status = rep.midStr("status=\"", "\"", false);
  if (status != "ok") {
    emit error();
    qDebug() << rep.toString();
    return;
  }

  const QString& key = rep.midStr("key=\"", "\"", false);

  emit got(key);
}

}
