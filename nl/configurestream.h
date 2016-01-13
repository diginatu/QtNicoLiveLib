#ifndef CONFIGURESTREAM_H
#define CONFIGURESTREAM_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"
#include "livewaku.h"

namespace nicolive {

/*!
 * \brief ConfigureStream に値を送信するクラスです。
 * 枠を取った後、放送を開始したり終了したりすることができます。
 */
class QTNICOLIVELIBSHARED_EXPORT ConfigureStream : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit ConfigureStream(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   * \param livewaku はfetchInformation()しておく必要があります。
   * \param value がisEmpty()の場合valueパラメータは省略されて送信されます。
   */
  void get(const QString& key, const QString& value,
           const LiveWaku& waku, const QString& userSession);

  /*!
   * \brief 放送を開始するように送信します。
   * get("exclude", "0", waku, userSession)と同等です。
   */
  void getStart(const LiveWaku& waku, const QString& userSession);
  /*!
   * \brief 放送を終了するように送信します。
   * get("end_now", "", waku, userSession)と同等です。
   */
  void getEnd(const LiveWaku& waku, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;

signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error();

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString key);
};

}

#endif // CONFIGURESTREAM_H
