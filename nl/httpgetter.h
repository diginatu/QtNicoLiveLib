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
protected:
  void requestGet(const QNetworkRequest& rq);
  void requestPost(const QNetworkRequest& rq, QIODevice* data);
  void get();

  QVariant makePostData(QString session_id);
  static QString htmlDecode(QString st);

  QNetworkAccessManager* mManager;

protected slots:
  void got(QNetworkReply* reply);
};

}

#endif // HTTPGETTER_H
