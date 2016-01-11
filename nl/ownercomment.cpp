#include "ownercomment.h"

namespace nicolive {

OwnerComment::OwnerComment(QObject *parent) : HttpGetter(parent)
{

}

void OwnerComment::get(const QString& text, const QString& name, const LiveWaku& waku, const QString& userSession)
{
  // make reques
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://watch.live.nicovideo.jp/api/broadcast/" + waku.getBroadID() +
                 "?body=" + QUrl::toPercentEncoding(text) +
                 "&token=" + waku.getOwnerCommentToken() +
                 "&name=" + QUrl::toPercentEncoding(name)));

  requestGet(rq);
}

void OwnerComment::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor rep(QString(reply->readAll()));

  const QString& status = rep.midStr("status=", "&");
  if ( status == "error" ) {
    emit error(rep.midStr("error=", ""));
    qDebug() << rep.toString();
    return;
  }

  emit got();
}

}
