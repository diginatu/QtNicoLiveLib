#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! PlayerStatus API から生放送の情報を取得するクラス。

class QTNICOLIVELIBSHARED_EXPORT PlayerStatus : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit PlayerStatus(QObject *parent = 0);

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
  void error(QString error);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString broadID, QString title, QString communityID, QString ownerID,
           QString ownerName, uint stTime, uint edTime, QString broadcastToken,
           QString userID, bool isPremium, QString addr, qint16 port,
           QString thread);
};

}

#endif // PLAYERSTATUS_H
