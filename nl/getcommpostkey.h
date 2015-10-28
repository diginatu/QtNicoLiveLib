#ifndef GETCOMMPOSTKEY_H
#define GETCOMMPOSTKEY_H

#include <QObject>

#include "httpgetter.h"

namespace nicolive {

class GetCommPostKey : public HttpGetter
{
  Q_OBJECT
public:
  explicit GetCommPostKey(QString thread, int blockNo, QString userSession, QObject *parent = 0);
  void get();
private:
  QString thread, userSession;
  int blockNo;

private slots:
  void gotReply(QNetworkReply* reply);
signals:
  void error();
  void got(QString postkey);

public slots:
};

}

#endif // GETCOMMPOSTKEY_H
