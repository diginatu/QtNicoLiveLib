#include "extend.h"

namespace nicolive {

Extend::Extend(QObject *parent) : HttpGetter(parent)
{
}

void Extend::get(const ExtendInfo::ExtendItem& extendItem, const QString& broadcastToken,
                 const QString& userSession)
{
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/usepoint/" + extendItem.broadID));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  QVariant postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setRawHeader("accept-language", "ja");

  QUrlQuery params;
  params.addQueryItem("num", extendItem.num);
  // params.addQueryItem("remain", "%2D1");
  params.addQueryItem("date", QString::number(QDateTime::currentDateTimeUtc().toTime_t()));
  params.addQueryItem("item", extendItem.item);
  params.addQueryItem("code", QUrl::toPercentEncoding(extendItem.code));
  params.addQueryItem("token", QUrl::toPercentEncoding(broadcastToken));

  requestPost(rq, params.toString(QUrl::FullyEncoded).toUtf8());
}

void Extend::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor data(QString(reply->readAll()));

  nicolive::StrAbstractor* errord = data.mid("<error>", "</error>");
  if (errord != nullptr) {
    const QString& code        = errord->midStr("<code>", "</code>");
    const QString& description = errord->midStr("<description>", "</description>");
    emit error(code, description);

    return;
  }

  if (data.midStr("<mode>", "</mode>") == "extend")
    emit got();
}

}
