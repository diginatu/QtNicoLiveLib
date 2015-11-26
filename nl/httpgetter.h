#ifndef HTTPGETTER_H
#define HTTPGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>

namespace nicolive {

class HttpGetter : public QObject
{
  Q_OBJECT
public:
  explicit HttpGetter(QObject *parent = 0);
  ~HttpGetter();

  static QString htmlDecode(QString st);
  QVariant makePostData(QString session_id);
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
