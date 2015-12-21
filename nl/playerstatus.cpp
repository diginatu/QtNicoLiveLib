#include "playerstatus.h"

namespace nicolive {

PlayerStatus::PlayerStatus(QObject *parent) :
  HttpGetter(parent)
{
}

void PlayerStatus::get(const QString& broadID, const QString& userSession)
{
  // make reques
  QNetworkRequest rq;
  const QVariant& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=" + broadID));

  requestGet(rq);
}

void PlayerStatus::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor commTcpi(QString(reply->readAll()));

  QString status = commTcpi.midStr("status=\"","\"");
  if (status != "ok") {
    QString code = commTcpi.midStr("<code>","</code>");
    error(code);
    return;
  }

  const QString& broadID = commTcpi.midStr("<id>", "</id>");
  const QString& title = commTcpi.midStr("<title>", "</title>");
  const QString& communityID = commTcpi.midStr("<default_community>", "</default_community>");
  const QString& ownerID = commTcpi.midStr("<owner_id>", "</owner_id>");
  const QString& ownerName = commTcpi.midStr("<owner_name>", "</owner_name>");
  const uint stTime = commTcpi.midStr("<start_time>","</start_time>").toUInt();
  const uint edTime = commTcpi.midStr("<end_time>","</end_time>").toUInt();
  const QString& broadcastToken = commTcpi.midStr("<broadcast_token>","</broadcast_token>");

  const QString& userID = commTcpi.midStr("<user_id>", "</user_id>");
  const bool isPremium = commTcpi.midStr("<is_premium>", "</is_premium>")!="0";

  const QString& addr = commTcpi.midStr("<addr>", "</addr>");
  const qint16 port = commTcpi.midStr("<port>", "</port>").toInt();
  const QString& thread = commTcpi.midStr("<thread>", "</thread>");

  emit got(broadID, title, communityID, ownerID, ownerName, stTime, edTime,
      broadcastToken, userID, isPremium, addr, port, thread);
}

}
