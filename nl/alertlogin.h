#ifndef ALERTLOGIN_H
#define ALERTLOGIN_H

#include <QObject>
#include <QUrlQuery>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! ニコ生アラートのログインをして AlertAdmin に必要なチケットを取得します

class QTNICOLIVELIBSHARED_EXPORT AlertLogin : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit AlertLogin(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& mailAddress, const QString& password);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString code, QString description);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString ticket);
};

}

#endif // ALERTLOGIN_H
