#ifndef STREAMINFO_H
#define STREAMINFO_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! 枠の情報（タイトル、説明、コミュニティ）を取得するクラスです。
class QTNICOLIVELIBSHARED_EXPORT StreamInfo : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit StreamInfo(QObject *parent = 0);

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
  void error();

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString broadID, QString title, QString description, QString communityID);
};

}

#endif // STREAMINFO_H
