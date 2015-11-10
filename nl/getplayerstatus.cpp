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
  QVariant postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/api/getplayerstatus?v=lv" + broadID));

  requestGet(rq);
}

void GetPlayerStatus::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor commTcpi(QString(reply->readAll()));

  QString status = commTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    QString code = commTcpi.midStr("<code>","</code>");
    error(code);
    reply->deleteLater();
    this->deleteLater();
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
  QString isPremium = commTcpi.midStr("<is_premium>", "</is_premium>");

  QString addr = commTcpi.midStr("<addr>", "</addr>");
  QString port = commTcpi.midStr("<port>", "</port>");
  QString thread = commTcpi.midStr("<thread>", "</thread>");

  got(broadID, title, communityID, ownerID, ownerName, stTime, edTime,
      broadcastToken, userID, isPremium, addr, port, thread);

  reply->deleteLater();
  this->deleteLater();
}

}
