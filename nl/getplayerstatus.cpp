#include "getplayerstatus.h"

namespace nicolive {

GetPlayerStatus::GetPlayerStatus(QString broadID, QString userSession, QObject *parent) :
  HttpGetter(parent)
{
  this->broadID = broadID;
  this->userSession = userSession;
}

void GetPlayerStatus::get()
{
  // make reques
  QNetworkRequest rq;
  QVariant&& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=" + broadID));

  requestGet(rq);
}

void GetPlayerStatus::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor commTcpi(QString(reply->readAll()));

  QString status = commTcpi.midStr("status=\"","\"");
  if (status != "ok") {
    QString code = commTcpi.midStr("<code>","</code>");
    error(code);
    return;
  }

  QString broadID = commTcpi.midStr("<id>", "</id>");
  QString title = commTcpi.midStr("<title>", "</title>");
  QString communityID = commTcpi.midStr("<default_community>", "</default_community>");
  QString ownerID = commTcpi.midStr("<owner_id>", "</owner_id>");
  QString ownerName = commTcpi.midStr("<owner_name>", "</owner_name>");
  uint stTime = commTcpi.midStr("<start_time>","</start_time>").toUInt();
  uint edTime = commTcpi.midStr("<end_time>","</end_time>").toUInt();
  QString broadcastToken = commTcpi.midStr("<broadcast_token>","</broadcast_token>");

  QString userID = commTcpi.midStr("<user_id>", "</user_id>");
  bool isPremium = commTcpi.midStr("<is_premium>", "</is_premium>")!="0";

  QString addr = commTcpi.midStr("<addr>", "</addr>");
  qint16 port = commTcpi.midStr("<port>", "</port>").toInt();
  QString thread = commTcpi.midStr("<thread>", "</thread>");

  emit got(broadID, title, communityID, ownerID, ownerName, stTime, edTime,
      broadcastToken, userID, isPremium, addr, port, thread);
}

}
