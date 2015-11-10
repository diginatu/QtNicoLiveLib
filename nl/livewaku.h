#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>

#include "getplayerstatus.h"

namespace nicolive {

class LiveWaku : public QObject
{
  Q_OBJECT
public:
  explicit LiveWaku(QObject *parent = 0);
  explicit LiveWaku(QString broadID, QObject *parent = 0);


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
  int getPort() const;

  bool getOwnerBroad() const;
  void setOwnerBroad(bool value);
  QString getOwnerCommentToken() const;


  void getInformation(QString userSession);

signals:
  gotInfornationError(QString code);
  gotInfornation();

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

  QString addr;
  QString thread;
  int port;

  bool ownerBroad;
  QString ownerCommentToken;

};

}

#endif // LIVEWAKU_H
