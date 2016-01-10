#ifndef COMMPOSTKEY_H
#define COMMPOSTKEY_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"

namespace nicolive {

//! コメント受信に必要なポストキーを取得するクラス。

class QTNICOLIVELIBSHARED_EXPORT CommPostKey : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit CommPostKey(QString thread, int blockNo,
                          const QString& userSession, QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get();
private:
  QString thread, userSession;
  int blockNo;
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error();

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString postkey);
};

}

#endif // COMMPOSTKEY_H
