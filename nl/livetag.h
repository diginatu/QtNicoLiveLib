#ifndef LIVETAG_H
#define LIVETAG_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! 生放送のタグを取得するクラス。

class QTNICOLIVELIBSHARED_EXPORT LiveTag : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit LiveTag(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   * ExtendInfo でリストが取得できます。
   */
  void get(const QString& broadID, const QString& userSession);

private:
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString code, QString description);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QStringList tags);
};

}

#endif // LIVETAG_H
