#ifndef FETCHFAVORITEDNOWWAKU_H
#define FETCHFAVORITEDNOWWAKU_H

#include <QObject>
#include "livewaku.h"
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! お気に入り登録しているコミュニティ等で、現在放送されている放送を取得します。
/*!
 * http://www.nicovideo.jp/my/live に表示されている放送を取得します。
 */
class QTNICOLIVELIBSHARED_EXPORT FetchFavoritedNowWaku : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit FetchFavoritedNowWaku(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   */
  void get(const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*!
   * 参考: get()
   */
  void error(QString error);

  //! 取得成功した場合にエミットされるシグナルです。
  /*!
   * \param mywaku には放送ID、コミュニティIDのみの情報が入っています。
   * deleteされる前にこのシグナルは複数回emitされる可能性があります。
   * また、myWakuは必要がなくなったらdeleteする必要があります。
   * 参考: get()
   */
  void got(LiveWaku* mywaku);
};

}

#endif // FETCHFAVORITEDNOWWAKU_H
