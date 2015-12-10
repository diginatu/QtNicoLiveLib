#include "alertlogin.h"

namespace nicolive {

AlertLogin::AlertLogin(QObject *parent) : HttpGetter(parent)
{
}

void AlertLogin::get(const QString& mailAddress, const QString& password)
{
  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("https://secure.nicovideo.jp/secure/login?site=nicolive_antenna"));
  rq.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QUrlQuery params;
  params.addQueryItem("next_url", "");
  params.addQueryItem("show_button_facebook", "0");
  params.addQueryItem("show_button_twitter", "0");
  params.addQueryItem("nolinks", "0");
  params.addQueryItem("_use_valid_error_code", "0");
  params.addQueryItem("mail", QUrl::toPercentEncoding(mailAddress));
  params.addQueryItem("password", QUrl::toPercentEncoding(password));

  requestPost(rq, params.toString(QUrl::FullyEncoded).toUtf8());
}

void AlertLogin::gotReply(QNetworkReply* reply)
{
  nicolive::StrAbstractor commTcpi(QString(reply->readAll()));

  QString status = commTcpi.midStr("status=\"","\"");
  if (status == "fail") {
    const QString code = commTcpi.midStr("<code>","</code>");
    const QString description = commTcpi.midStr("<description>","</description>");
    emit error(code, description);
    return;
  }

  const QString ticket = commTcpi.midStr("<ticket>","</ticket>");
  if (ticket.isEmpty()) {
    emit error("ticket", "incorrect account");
    return;
  }
  emit got(ticket);
}

}
