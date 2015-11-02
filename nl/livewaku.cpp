#include "livewaku.h"

namespace nicolive {

LiveWaku::LiveWaku(QObject *parent) : QObject(parent)
{

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

QString LiveWaku::getUserId() const { return userId; }
QString LiveWaku::getOwnerId() const { return ownerId; }
bool LiveWaku::getUserPremium() const { return userPremium; }

QString LiveWaku::getPostKey() const { return postKey; }

QString LiveWaku::getAddr() const { return addr; }
QString LiveWaku::getThread() const { return thread; }
int LiveWaku::getPort() const { return port; }

bool LiveWaku::getOwnerBroad() const { return ownerBroad; }
void LiveWaku::setOwnerBroad(bool value) { ownerBroad = value; }
QString LiveWaku::getOwnerCommentToken() const { return ownerCommentToken; }

}
