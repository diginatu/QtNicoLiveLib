#ifndef PUBLISHSTATUS_H
#define PUBLISHSTATUS_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! 主コメントに必要なチケットを取得します。
/*!
 * \see OwnerComment
 */
class QTNICOLIVELIBSHARED_EXPORT PublishStatus : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit PublishStatus(QObject* parent);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& broadID, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;

signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString code);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString token);
};

}

#endif // PUBLISHSTATUS_H
