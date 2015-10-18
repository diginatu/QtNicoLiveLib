#include "fetchusername.h"

FetchUserName::FetchUserName(QObject* parent, QString userID) :
  HttpGetter(parent)
{
  this->userID = userID;
}

FetchUserName::~FetchUserName()
{
}

void FetchUserName::get()
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(gotReply(QNetworkReply*)));

  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("http://api.ce.nicovideo.jp/api/v1/user.info?user_id=" + userID));

  mManager->get(rq);
}

void FetchUserName::gotReply(QNetworkReply* reply)
{
  StrAbstractor userinfo(QString(reply->readAll()));

  QString status = userinfo.midStr("status=\"", "\">");

  if (status == "fail") {
    // QMessageBox::information(mwin, "Viqo", QStringLiteral("ユーザが見つかりませんでした"));
    return;
  }

  QString username = userinfo.midStr("<nickname>", "</nickname>");
  emit got(username);

  reply->deleteLater();
  this->deleteLater();
}
