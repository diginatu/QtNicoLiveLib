#ifndef USERSESSIONLOGIN_H
#define USERSESSIONLOGIN_H

#include <QObject>
#include <QUrlQuery>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! ニコ動にログインしてユーザセッションを取得します

class UserSessionLogin : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit UserSessionLogin(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalのgotまたはerrorをemitします。
   */
  void get(const QString& mailAddress, const QString& password);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error();

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString userSession);
};

}

#endif // USERSESSIONLOGIN_H
