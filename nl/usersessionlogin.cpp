#include "usersessionlogin.h"

namespace nicolive {

UserSessionLogin::UserSessionLogin(QObject *parent) : HttpGetter(parent)
{
}

void UserSessionLogin::get(const QString& mailAddress, const QString& password)
{
  // make request
  QNetworkRequest rq;
  rq.setUrl(QUrl("https://secure.nicovideo.jp/secure/login?site=nicolive"));
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

void UserSessionLogin::gotReply(QNetworkReply* reply)
{
  auto headers = reply->rawHeaderPairs();

  bool success = false;
  for (auto& header : headers) {
    if (header.first == "Set-Cookie") {
      auto cookies = QNetworkCookie::parseCookies(header.second);
      for (auto& cookie : cookies) {
        if (cookie.name() == "user_session" &&
            cookie.value() != "deleted" &&
            cookie.value() != "") {
          emit got(cookie.value());
          success = true;
          break;
        }
      }
      break;
    }
  }

  if (!success) emit error();
}

}
