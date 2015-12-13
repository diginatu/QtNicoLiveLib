#ifndef HTTPGETTER_H
#define HTTPGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>
#include "qtnicolivelib.h"

namespace nicolive {

//! APIを取得するのに使用されている抽象クラス。
/*!
 * requestGetまたはrequestPostが呼ばれた後、
 * gotReplyがemitされその後本クラスはdeleteされる。
 * そのため、本クラスの抽象クラスはnewして作成されるべきである。
 */

class QTNICOLIVELIBSHARED_EXPORT HttpGetter : public QObject
{
  Q_OBJECT
public:
  explicit HttpGetter(QObject *parent = 0);
  ~HttpGetter();

  static QString htmlDecode(QString st);
  QVariant makePostData(const QString& sessionId);
private slots:
  void preGot(QNetworkReply* reply);
protected:
  void requestGet(const QNetworkRequest& rq);
  void requestPost(const QNetworkRequest& rq, const QByteArray& data);
  virtual void gotReply(QNetworkReply* reply) = 0;

  QNetworkAccessManager* mManager;
protected slots:
};

}

#endif // HTTPGETTER_H
