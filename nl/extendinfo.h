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
  explicit ExtendInfo(QObject* parent = 0);

  struct ExtendItem { QString label, price, num, code, item, broadID; };

  void get(const QString& broadID, const QString& userSession);
private:
  void gotReply(QNetworkReply* reply) override;
  QString broadID;
signals:
  void error(QString code, QString description);
  void got(QVector<ExtendItem> extendList);
};

}

#endif // EXTENDINFO_H
