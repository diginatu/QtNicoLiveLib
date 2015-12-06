#ifndef ALERTADMIN_H
#define ALERTADMIN_H

#include <QObject>
#include <QUrlQuery>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! ニコ生アラート取得のために必要な情報と、登録コミュニティを取得します。

class AlertAdmin : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit AlertAdmin(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& ticket);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString code);

  //! 取得成功した場合にエミットされるシグナルです。
  /*!
   * \arg myCommunities はコミュニティID("co1234", "ch1234"など)のQStringListです。
   *
   * 参考: get()
   */
  void got(QStringList myCommunities, QString addr, qint16 port, QString thread);
};

}

#endif // ALERTADMIN_H
