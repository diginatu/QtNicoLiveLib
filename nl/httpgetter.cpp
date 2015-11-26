#include "httpgetter.h"

namespace nicolive {

HttpGetter::HttpGetter(QObject *parent) :
  QObject(parent),
  mManager(nullptr)
{
}

HttpGetter::~HttpGetter()
{
  if(mManager!=nullptr) delete mManager;
}

void HttpGetter::requestGet(const QNetworkRequest& rq)
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(preGot(QNetworkReply*)));

  mManager->get(rq);
}

void HttpGetter::requestPost(const QNetworkRequest& rq, const QByteArray& data)
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(preGot(QNetworkReply*)));

  mManager->post(rq, data);
}

void HttpGetter::preGot(QNetworkReply* reply)
{
  gotReply(reply);
  reply->deleteLater();
  this->deleteLater();
}

QVariant HttpGetter::makePostData(QString session_id)
{
  QVariant postData;

  // make cookies
  QList <QNetworkCookie> cookies;
  QNetworkCookie ck;
  ck.toRawForm(QNetworkCookie::NameAndValueOnly);
  ck.setName("user_session");

  QByteArray user_id_ba;
  user_id_ba.append(session_id);

  ck.setValue(user_id_ba);
  cookies.append(ck);

  postData.setValue(cookies);
  return postData;
}

QString HttpGetter::htmlDecode(QString st)
{
  st.replace("&amp;", "&");
  st.replace("&lt;", "<");
  st.replace("&gt;", ">");
  return st;
}

}
