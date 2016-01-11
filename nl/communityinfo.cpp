#include "communityinfo.h"

namespace nicolive {

CommunityInfo::CommunityInfo(QObject *parent) : HttpGetter(parent)
{

}

void CommunityInfo::get(const QString& commID, const QString userSession)
{
  // make reques
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://com.nicovideo.jp/community/" + commID));

  requestGet(rq);
}

void CommunityInfo::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor communityData(QString(reply->readAll()));

  const QString title = communityData.midStr("<h1 id=\"community_name\">", "</h1>");
  if (title.isEmpty()) {
    emit error();
    return;
  }

  emit got(title);
}

}
