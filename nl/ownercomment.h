#ifndef OWNERCOMMENT_H
#define OWNERCOMMENT_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"
#include "livewaku.h"

namespace nicolive {

//! 主コメントをするクラスです。
/*!
 * トークンを取得する必要があります。
 * \see PublishStatus
 */
class QTNICOLIVELIBSHARED_EXPORT OwnerComment : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit OwnerComment(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * \param waku はfetchOwnerCommentToken()しておく必要があります。
   *
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& text, const QString& name, const LiveWaku& waku, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;

signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString error);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got();
};

}

#endif // OWNERCOMMENT_H
