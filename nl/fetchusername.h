#ifndef FETCHUSERNAME_H
#define FETCHUSERNAME_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

//! ユーザIDからユーザ名を取得するクラス。
/*!
 * 標準入力からユーザIDを受け取りユーザ名を表示する例。
 * \include FetchUserName/testclass.cpp
 */

class FetchUserName : public HttpGetter
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit FetchUserName(QString userID, QObject* parent = 0);

  //! リクエストを発行します。
  /*!
   * 取得後、Signalのgotまたはerrorをemitします。
   */
  void get();
private:
  QString userID;
  void gotReply(QNetworkReply* reply) override;
signals:
  //! エラーになった場合にエミットされるシグナルです。
  /*! 参考: get() */
  void error(QString code, QString description);

  //! 取得成功した場合にエミットされるシグナルです。
  /*! 参考: get() */
  void got(QString username);

};

}

#endif // FETCHUSERNAME_H
