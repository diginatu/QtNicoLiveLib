#ifndef EXTEND_H
#define EXTEND_H

#include <QObject>
#include <QUrlQuery>
#include "extendinfo.h"
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

class Extend : public HttpGetter
{
  Q_OBJECT
public:
  explicit Extend(QObject *parent = 0);

  void get(const ExtendInfo::ExtendItem& extendItem, const QString& broadcastToken,
           const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  void error(QString code, QString description);
  void got();
};

}

#endif // EXTEND_H
