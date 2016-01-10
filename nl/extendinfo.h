#ifndef EXTENDINFO_H
#define EXTENDINFO_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! ニコ生APIで取得される延長情報に放送IDを追加した延長項目を保持するクラス。
struct QTNICOLIVELIBSHARED_EXPORT ExtendItem { QString label, price, num, code, item, broadID; };

//! ニコ生放送の延長情報を取得するクラス。

class QTNICOLIVELIBSHARED_EXPORT ExtendInfo : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit ExtendInfo(QObject* parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& broadID, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;
  QString broadID;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*!
   * 参考: get()
   */
  void error(QString code, QString description);

  //! 取得成功した場合にエミットされるシグナルです。
  /*!
   * deleteされる前にこのシグナルは複数回emitされる可能性があります。
   * 参考: get()
   */
  void got(ExtendItem extendItem);
};

}

#endif // EXTENDINFO_H
