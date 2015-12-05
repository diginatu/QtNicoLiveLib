#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

#include "getplayerstatus.h"

namespace nicolive {

class LiveWaku : public QObject
{
  Q_OBJECT
public:
  explicit LiveWaku(QObject *parent = 0);
  explicit LiveWaku(const QString& broadID, QObject *parent = 0);


  // setter and getter
  QString getTitle() const;
  void setTitle(const QString& value);
  QString getBroadID() const;
  void setBroadID(const QString& value);
  QString getCommunityID() const;
  void setCommunityID(const QString& value);
  QString getOwnerName() const;
  void setOwnerName(const QString& value);
  QString getBroadcastToken() const;

  QDateTime getStTime() const;
  void setStTime(const QDateTime& value);
  QDateTime getEdTime() const;
  void setEdTime(const QDateTime& value);

  QString getUserID() const;
  QString getOwnerID() const;
  bool getUserPremium() const;

  QString getPostKey() const;

  QString getAddr() const;
  QString getThread() const;
  qint16 getPort() const;

  bool getOwnerBroad() const;
  void setOwnerBroad(bool value);
  QString getOwnerCommentToken() const;


  void getInformation(const QString& userSession);

signals:
  void gotInfornationError(QString code);
  void gotInfornation();

private:
  QString title;
  QString broadID;
  QString communityID;
  QString ownerName;
  QString broadcastToken;

  QDateTime stTime;
  QDateTime edTime;

  QString userID, ownerID;
  bool userPremium;

  QString postKey;

  QString addr, thread;
  qint16 port;

  bool ownerBroad;
  QString ownerCommentToken;

};

}

#endif // LIVEWAKU_H
