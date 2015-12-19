#include "fetchfavoritednowwaku.h"

namespace nicolive {

FetchFavoritedNowWaku::FetchFavoritedNowWaku(QObject *parent) :
  HttpGetter(parent)
{

}

void FetchFavoritedNowWaku::get(const QString& userSession)
{
  // make request
  QNetworkRequest rq;
  QVariant&& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://www.nicovideo.jp/my/live"));

  requestGet(rq);
}

void FetchFavoritedNowWaku::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor liveID(QString(reply->readAll()));

  if (liveID.toString().isEmpty()) {
    qDebug() << "The result is empty. you may be not logged in.";
    emit error("The result is empty. you may be not logged in.");
    return;
  }

  // seek to Programs from the joined channels/communities if exist
  if (liveID.forward("<div class=\"articleBody \" id=\"ch\">") == -1) {
    return;
  }

  QString community;
  while((community = liveID.midStr("http://com.nicovideo.jp/community/", "\">")) != "") {
    const QString& ID = liveID.midStr("http://live.nicovideo.jp/watch/", "?ref=");

    auto awaku = new LiveWaku(ID);
    awaku->setCommunityID(community);
    emit got(awaku);
  }
}

}
