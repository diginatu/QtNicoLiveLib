#include "livetag.h"

namespace nicolive {

LiveTag::LiveTag(QObject *parent) : HttpGetter(parent)
{
}

void LiveTag::get(const QString& broadID, const QString& userSession)
{
  // make request
  QNetworkRequest rq;
  QVariant&& postData = makePostData(userSession);
  rq.setHeader(QNetworkRequest::CookieHeader, postData);
  rq.setUrl(QUrl("http://live.nicovideo.jp/livetags.php?v=lv" + broadID));

  requestGet(rq);
}

void LiveTag::gotReply(QNetworkReply* reply)
{
  StrAbstractor allTagHtml(QString(reply->readAll()));
  auto tagsabs = allTagHtml.mid("<table>", "</table>");

  QStringList tags;

  while (auto tagabs = tagsabs->mid("<nobr>", "</nobr>")) {
    const QString tag = tagabs->midStr(">", "</a>");
    tags << tag;
  }

  emit got(tags);
}

}
