#include "streaminfo.h"

namespace nicolive {

StreamInfo::StreamInfo(QObject *parent) : HttpGetter(parent)
{
}

void StreamInfo::get(const QString& broadID, const QString& userSession)
{
  // make request
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getstreaminfo/lv" + broadID));

  requestGet(rq);
}

void StreamInfo::gotReply(QNetworkReply* reply)
{
  StrAbstractor receivedData(QString(reply->readAll()));

  const QString status = receivedData.midStr("status=\"", "\"");
  if ( status == "fail" ) {
    emit error();
    return;
  }

  const QString& broadID = receivedData.midStr("<request_id>lv", "</request_id>");
  const QString& title = receivedData.midStr("<title>", "</title>");
  const QString& description = receivedData.midStr("<description>", "</description>");
  const QString& communityID = receivedData.midStr("<default_community>co", "</default_community>");

  emit got(broadID, title, description, communityID);
}

}
