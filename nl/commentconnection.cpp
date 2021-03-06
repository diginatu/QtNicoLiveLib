﻿#include "commentconnection.h"

namespace nicolive {

CommentConnection::CommentConnection(LiveWaku* livewaku,
                                     const QString& userSession, QObject *parent) :
  QObject(parent),
  lastBlockNum(0),
  userSession(userSession)
{
  socket = new QTcpSocket(this);
  this->livewaku = livewaku;

  nullDataTimer = new QTimer(this);
  postkeyTimer = new QTimer(this);

  reconnectTimes = 3;
  reconnectN = 0;
  reconnectWaitMsec = 1000;

  connect(socket, SIGNAL(connected()), this, SLOT(connected()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(connectionErrorOccured()));

  connect(nullDataTimer, SIGNAL(timeout()), this, SLOT(sendNull()));
  connect(postkeyTimer, &QTimer::timeout, this, [=](){
    livewaku->fetchPostKey(lastBlockNum, userSession);
  });
}

CommentConnection::~CommentConnection()
{
  close();
}

void CommentConnection::doConnect()
{
  const QString& addr = livewaku->getAddr();
  const qint16& port = livewaku->getPort();
  if (addr.isEmpty()) {
    emit error("doConnect", "livewaku don't have information to connect");
  }
  socket->connectToHost(addr, port);
}

void CommentConnection::close()
{
  nullDataTimer->stop();
  postkeyTimer->stop();
  socket->close();
}

void CommentConnection::sendComment(QString text, bool iyayo)
{
  const auto startTime = livewaku->getStTime().toTime_t();
  const auto nowTime = QDateTime::currentDateTime();

  const QString& postkey = livewaku->getPostKey();
  if (postkey.isEmpty()) {
    emit error("sendComment", "no postKey in livewaku");
    return;
  }

  QByteArray send;

  const uint vpos = 100 * (serverTime.toTime_t() - startTime +
                     nowTime.toTime_t() - openTime.toTime_t());

  send.append(QString("<chat thread=\"%1\" ticket=\"%2\" vpos=\"%3\" "
          "postkey=\"%4\"%5 user_id=\"%6\"%7>%8</chat>")
      .arg(livewaku->getThread())
      .arg(ticket)
      .arg(vpos)
      .arg(postkey)
      .arg(iyayo?" mail=\"184\"":"")
      .arg(livewaku->getUserID())
      .arg(livewaku->getUserPremium()?" premium=\"1\"":"")
      .arg(text.toHtmlEscaped()));

  send.append('\0');

  if (socket->write(send) == -1) {
    emit error("sendComment", socket->errorString());
    return;
  }
}

QDateTime CommentConnection::getOpenTime() const
{
  return openTime;
}

void CommentConnection::connected()
{
  QByteArray send;

  send.append("<thread thread=\"");
  send.append(livewaku->getThread());
  send.append("\" res_from=\"-1000\" version=\"20061206\" />");
  send.append('\0');

  if (socket->write(send) == -1) {
    close();
    ++ reconnectN;
    if (reconnectN < reconnectTimes) {
      QTimer::singleShot(reconnectWaitMsec, Qt::VeryCoarseTimer, [=](){ doConnect(); });
    } else {
      emit error("connected", socket->errorString());
    }
    return;
  }
  reconnectN = 0;

  openTime = QDateTime::currentDateTime();
  emit socketConnected();

  // set timer to send NULL data.
  nullDataTimer->start(60000);
}

void CommentConnection::disconnected()
{
  emit socketDisconnected();
}

void CommentConnection::readyRead()
{
  const QList<QByteArray>& rawcomms( socket->readAll().split('\0') );
  if (rawcomms.isEmpty()) return;

  if (rawcomms.size() > 1) {
    emit readOneRawComment(lastRawComm + rawcomms[0]);
    for ( int i = 1; i < rawcomms.size()-1; ++i) {
      emit readOneRawComment(rawcomms[i]);
    }
  }

  lastRawComm = rawcomms.last();
}

void CommentConnection::readOneRawComment(const QString& rawcomm)
{
  nicolive::StrAbstractor rawcommabs(rawcomm);

  if (rawcomm.startsWith("<thread")) {
    lastBlockNum = rawcommabs.midStr("last_res=\"", "\"", false).toInt() / 10;
    ticket = rawcommabs.midStr("ticket=\"", "\"", false);
    serverTime.setTime_t(rawcommabs.midStr("server_time=\"", "\"", false).toUInt());

    livewaku->fetchPostKey(lastBlockNum, userSession);

    // set timer to get post_key
    postkeyTimer->start(10000);

    return;
  }

  if (rawcomm.startsWith("<chat_result")) {
    const auto status = rawcommabs.midStr("status=\"", "\"", false);
    if (status != "0") {
      emit error("readOneRawComment", status);
    } else {
      emit submittedComment();
    }
    return;
  }

  nicolive::StrAbstractor* comminfo = rawcommabs.mid("<chat", ">");
  rawcommabs.setRelativePosition(-2);

  QString comment = rawcommabs.midStr(">", "</chat>");
  comment = nicolive::HttpGetter::htmlDecode(comment);

  int num = comminfo->midStr("no=\"", "\"", false).toInt();
  const int block = num/10;
  if (block > lastBlockNum) {
    lastBlockNum = block;

    livewaku->fetchPostKey(lastBlockNum, userSession);
  }

  QDateTime commentTime;
  commentTime.setTime_t(comminfo->midStr("date=\"", "\"", false).toUInt());

  QString mail = comminfo->midStr("mail=\"", "\"", false);
  bool iyayo = (mail.indexOf("184")!=-1) ? true : false;

  QString userID = comminfo->midStr("user_id=\"", "\"", false);

  bool premium = false, broadcaster = false;
  QString premS = comminfo->midStr("premium=\"", "\"", false);
  if ( !premS.isEmpty() ) {
    int prem = premS.toInt();
    if (prem % 2) premium = true;
    if ((prem >> 1) % 2 ) broadcaster = true;
  }

  emit newComment(num, premium, userID, comment, commentTime, iyayo, broadcaster);

  if (comment == "/disconnect" && broadcaster) {
    close();
  }
}

void CommentConnection::connectionErrorOccured()
{
  close();
  ++ reconnectN;
  if (reconnectN < reconnectTimes) {
    QTimer::singleShot(reconnectWaitMsec, Qt::VeryCoarseTimer, [=](){ doConnect(); });
  } else {
    emit error("QAbstractSocket::SocketError", socket->errorString());
  }
}

bool CommentConnection::isConnected()
{
  if (socket == nullptr) return false;
  return socket->state() != QAbstractSocket::UnconnectedState;
}

void CommentConnection::sendNull()
{
  QByteArray send(1, '\0');

  if (socket->write(send) == -1) {
    emit error("sendNull", socket->errorString());
    socket->close();
    doConnect();
  }
}

}
