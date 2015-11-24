#ifndef GETCOMMPOSTKEY_H
#define GETCOMMPOSTKEY_H

#include <QObject>

#include "httpgetter.h"

namespace nicolive {

class GetCommPostKey : public HttpGetter
{
  Q_OBJECT
public:
  explicit GetCommPostKey(QString thread, int blockNo,
                          const QString& userSession, QObject *parent = 0);
  void get();
private:
  QString thread, userSession;
  int blockNo;
  void gotReply(QNetworkReply* reply) override;
signals:
  void error();
  void got(QString postkey);
};

}

#endif // GETCOMMPOSTKEY_H
