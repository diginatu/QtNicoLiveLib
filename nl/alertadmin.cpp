#include "alertadmin.h"

namespace nicolive {

AlertAdmin::AlertAdmin(QObject *parent) : HttpGetter(parent)
{
}

void AlertAdmin::get(const QString& ticket)
{
  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getalertstatus"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem("ticket", ticket);

  requestPost(rq, params.toString(QUrl::FullyEncoded).toUtf8());
}

void AlertAdmin::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor wakuTcpi(QString(reply->readAll()));

  QString status = wakuTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    const QString code = wakuTcpi.midStr("<code>","</code>");
    emit error(code);
    return;
  }

  nicolive::StrAbstractor* communityi = wakuTcpi.mid("<communities>","</communities>");

  QStringList myCommunities;
  QString mycommunity;
  while (!(mycommunity = communityi->midStr("<community_id>","</community_id>")).isNull()) {
    myCommunities << mycommunity;
  }

  const QString addr = wakuTcpi.midStr("<addr>", "</addr>");
  const qint16 port = wakuTcpi.midStr("<port>", "</port>").toInt();
  const QString thread = wakuTcpi.midStr("<thread>", "</thread>");

  emit got(myCommunities, addr, port, thread);
}

}
