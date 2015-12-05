#ifndef GETPLAYERSTATUS_H
#define GETPLAYERSTATUS_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

class GetPlayerStatus : public HttpGetter
{
  Q_OBJECT
public:
  explicit GetPlayerStatus(QString broadID, QString userSession, QObject *parent = 0);

  void get();

private:
  QString broadID, userSession;
  void gotReply(QNetworkReply* reply) override;

signals:
  void error(QString error);
  void got(QString broadID, QString title, QString communityID, QString ownerID,
           QString ownerName, uint stTime, uint edTime, QString broadcastToken,
           QString userID, bool isPremium, QString addr, qint16 port,
           QString thread);
};

}

#endif // GETPLAYERSTATUS_H
