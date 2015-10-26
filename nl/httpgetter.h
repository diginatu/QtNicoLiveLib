#ifndef HTTPGETTER_H
#define HTTPGETTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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
  QNetworkAccessManager* mManager;

private:

signals:

public slots:

private slots:

protected slots:
  void got(QNetworkReply* reply);
};

}

#endif // HTTPGETTER_H
