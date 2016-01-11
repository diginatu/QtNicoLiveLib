#ifndef COMMUNITYINFO_H
#define COMMUNITYINFO_H

#include <QObject>
#include "qtnicolivelib.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! コミュニティIDからコミュニティの情報を取得するクラスです。
class QTNICOLIVELIBSHARED_EXPORT CommunityInfo : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit CommunityInfo(QObject *parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalの got() または error() をemitします。
   * \param livewaku はfetchInformation()しておく必要があります。
   */
  void get(const QString& commID, const QString userSession);
private:
  void gotReply(QNetworkReply* reply) override;

signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error();

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString title);
};

}

#endif // COMMUNITYINFO_H
