#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! 来場者数やコメント数を取得するクラスです。

class QTNICOLIVELIBSHARED_EXPORT HeartBeat : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit HeartBeat(QObject *parent = 0);

  /*!
   * \brief リクエストを発行します。
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& broadID, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;

signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString error, QString description);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString watchCount, QString commentCount);
};

}

#endif // HEARTBEAT_H
