#ifndef EXTENDINFO_H
#define EXTENDINFO_H

#include <QObject>
#include "httpgetter.h"
#include "strabstractor.h"

namespace nicolive {

class ExtendInfo : public HttpGetter
{
  Q_OBJECT
public:
  ExtendInfo(QObject* parent);

  void get(const QString& broadID, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;
signals:
  void error(QString code, QString description);
  void got(QVector< QVector<QString> > extendList);
};

}

#endif // EXTENDINFO_H
