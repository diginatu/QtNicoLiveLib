#include "extendinfo.h"

namespace nicolive {

ExtendInfo::ExtendInfo(QObject* parent) : HttpGetter(parent)
{
}

void ExtendInfo::get(const QString& broadID, const QString& userSession)
{
  this->broadID = broadID;

  // make request
  QNetworkRequest rq;
  QVariant postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/getsalelist?v=" + broadID));

  requestGet(rq);
}

void ExtendInfo::gotReply(QNetworkReply* reply)
{
  StrAbstractor data(QString(reply->readAll()));

  QString status = data.midStr("status=\"", "\"");

  if (status != "ok") {
    QString code = data.midStr("<code>", "</code>");
    QString description = data.midStr("<description>", "</description>");
    emit error(code, description);

    qDebug() << data.toString();
    return;
  }

  QVector<ExtendItem> extendList;

  StrAbstractor* item;
  while ((item = data.mid("<item>", "</item>")) != nullptr) {
    QString label     = item->midStr("<label>", "</label>");
    QString price     = item->midStr("<price>", "</price>");
    QString num       = item->midStr("<num>",   "</num>");
    QString code      = item->midStr("<code>",  "</code>");
    QString itemType  = item->midStr("<item>",  "");

    extendList.append(ExtendItem{label, code, itemType, num, price, broadID});
  }

  emit got(extendList);
}

}
