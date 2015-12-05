#ifndef EXTEND_H
#define EXTEND_H

#include <QObject>
#include <QUrlQuery>
#include "extendinfo.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! ニコ生放送の延長を実行するクラス。
/*! 参考: ExtendInfo */

class Extend : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit Extend(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalのgotまたはerrorをemitします。
   * ExtendInfo でリストが取得できます。
   */
  void get(const ExtendItem& extendItem, const QString& broadcastToken,
           const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString code, QString description);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got();
};

}

#endif // EXTEND_H
