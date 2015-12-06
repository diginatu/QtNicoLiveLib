#ifndef GETPLAYERSTATUS_H
#define GETPLAYERSTATUS_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! PlayerStatus API から生放送の情報を取得するクラス。

class GetPlayerStatus : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit GetPlayerStatus(QString broadID, QString userSession, QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get();

private:
  QString broadID, userSession;
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

#endif // GETPLAYERSTATUS_H
