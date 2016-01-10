#ifndef CONFIGURESTREAM_H
#define CONFIGURESTREAM_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"
#include "livewaku.h"

namespace nicolive {

//! ConfigureStream に値を送信するクラスです。
class QTNICOLIVELIBSHARED_EXPORT ConfigureStream : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit ConfigureStream(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   * \param livewaku はfetchInformation()しておく必要があります。
   */
  void get(const QString& key, const QString& value,
           const LiveWaku& waku, const QString userSession);
private:
  void gotReply(QNetworkReply* reply) override;

signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error();

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString key);
};

}

#endif // CONFIGURESTREAM_H
