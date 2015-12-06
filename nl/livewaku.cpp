#include "livewaku.h"

namespace nicolive {

LiveWaku::LiveWaku(QObject *parent) : QObject(parent) { }
LiveWaku::LiveWaku(const QString& broadID, QObject* parent) : LiveWaku(parent)
{
  this->broadID = broadID;
}


// setter and getter
QString LiveWaku::getTitle() const { return title; }
void LiveWaku::setTitle(const QString& value) { title = value; }
QString LiveWaku::getBroadID() const { return broadID; }
void LiveWaku::setBroadID(const QString& value) { broadID = value; }
QString LiveWaku::getCommunityID() const { return communityID; }
void LiveWaku::setCommunityID(const QString& value) { communityID = value; }
QString LiveWaku::getOwnerName() const { return ownerName; }
void LiveWaku::setOwnerName(const QString& value) { ownerName = value; }
QString LiveWaku::getBroadcastToken() const { return broadcastToken; }

QDateTime LiveWaku::getStTime() const { return stTime; }
void LiveWaku::setStTime(const QDateTime& value) { stTime = value; }
QDateTime LiveWaku::getEdTime() const { return edTime; }
void LiveWaku::setEdTime(const QDateTime& value) { edTime = value; }

QString LiveWaku::getUserID() const { return userID; }
QString LiveWaku::getOwnerID() const { return ownerID; }
bool LiveWaku::getUserPremium() const { return userPremium; }

QString LiveWaku::getPostKey() const { return postKey; }

QString LiveWaku::getAddr() const { return addr; }
QString LiveWaku::getThread() const { return thread; }
qint16 LiveWaku::getPort() const { return port; }

bool LiveWaku::getOwnerBroad() const { return ownerBroad; }
void LiveWaku::setOwnerBroad(bool value) { ownerBroad = value; }
QString LiveWaku::getOwnerCommentToken() const { return ownerCommentToken; }



void LiveWaku::fetchInformation(const QString& userSession)
{
  if (broadID.isEmpty()) {
    qDebug() << "LiveWaku::getInformation : broadID not set";
    return;
  }

  auto gt = new GetPlayerStatus(broadID, userSession, this);
  connect(gt, &GetPlayerStatus::error, this, [&](QString code){
    emit gotInfornationError(code);
  });
  connect(gt, &GetPlayerStatus::got, this, [&](QString broadID, QString title,
          QString communityID, QString ownerID, QString ownerName, uint stTime,
          uint edTime, QString broadcastToken, QString userID, bool isPremium,
          QString  addr, qint16 port, QString thread)
  {
    this->broadID = broadID;
    this->title = title;
    this->communityID = communityID;
    this->ownerID = ownerID;
    this->ownerName = ownerName;
    this->stTime.setTime_t(stTime);
    this->edTime.setTime_t(edTime);
    this->broadcastToken = broadcastToken;
    this->userID = userID;
    this->userPremium = isPremium;
    this->addr = addr;
    this->port = port;
    this->thread = thread;

    emit gotInfornation();
  });
  gt->get();
}

void LiveWaku::fetchPostKey(int lastBlockNum, const QString& userSession)
{
    auto gt = new nicolive::GetCommPostKey(thread, lastBlockNum, userSession, this);
    connect(gt, &nicolive::GetCommPostKey::error, this, [=](){
      emit gotPostKeyError();
    });
    connect(gt, &nicolive::GetCommPostKey::got, this, [=](QString postKey){
      this->postKey = postKey;
      emit gotPostKey();
    });
    gt->get();
}

}
