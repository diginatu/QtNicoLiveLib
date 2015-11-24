#ifndef FETCHUSERNAME_H
#define FETCHUSERNAME_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

class FetchUserName : public HttpGetter
{
  Q_OBJECT
public:
  explicit FetchUserName(QString userID, QObject* parent = 0);
  ~FetchUserName();

  void get();
private:
  QString userID;
  void gotReply(QNetworkReply* reply) override;
signals:
  void error(QString code, QString description);
  void got(QString username);

};

}

#endif // FETCHUSERNAME_H
