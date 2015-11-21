#include "commentconnection.h"

namespace nicolive {

CommentConnection::CommentConnection(LiveWaku* livewaku,
                                     const QString& userSession, QObject *parent) :
  QObject(parent),
  lastBlockNum(0),
  userSession(userSession)
{
  socket = new QTcpSocket(this);
  this->livewaku = livewaku;

  connect(socket, SIGNAL(connected()),this, SLOT(connected()));
  connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

  connect(&nullDataTimer, SIGNAL(timeout()), this, SLOT(sendNull()));
  connect(&postkeyTimer, SIGNAL(timeout()), this, SLOT(getPostKey()));
}

CommentConnection::~CommentConnection()
{
  close();
}

void CommentConnection::doConnect()
{
  const QString& addr = livewaku->getAddr();
  const uint port = livewaku->getPort().toUInt();
  socket->connectToHost(addr, port);

  if(!socket->waitForConnected(5000)) {
    emit error("connect", socket->errorString());
  }
}

void CommentConnection::close()
{
  nullDataTimer.stop();
  postkeyTimer.stop();
  socket->close();
}

void CommentConnection::sendComment(QString text, bool iyayo)
{
  const auto startTime = livewaku->getStTime().toTime_t();
  const auto nowTime = QDateTime::currentDateTime();

  QString postkey = livewaku->getPostKey();
  if (postkey.isEmpty()) {
    emit error("CommentConnection::sendComment", "no postKey");
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
    emit error("CommentConnection::sendComment", socket->errorString());
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
    emit error("connected", socket->errorString());
    doConnect();
  }

  // set timer to send NULL data.
  nullDataTimer.start(60000);
}

void CommentConnection::disconnected()
{
  emit connectionClosed();
}

void CommentConnection::readyRead()
{
  QList<QByteArray> rawcomms( socket->readAll().split('\0') );
  rawcomms[0].insert(0, lastRawComm);

  for ( int i = 0; i < rawcomms.size()-1; ++i) {
    const QString tst(rawcomms[i]);
    readOneRawComment(tst);
  }

  lastRawComm = rawcomms.takeLast();
}

void CommentConnection::readOneRawComment(const QString& rawcomm)
{
  nicolive::StrAbstractor rawcommabs(rawcomm);

  if (rawcomm.startsWith("<thread")) {
    openTime = QDateTime::currentDateTime();
    emit connectionStarted();

    lastBlockNum = rawcommabs.midStr("last_res=\"", "\"", false).toInt() / 10;
    ticket = rawcommabs.midStr("ticket=\"", "\"", false);
    serverTime.setTime_t(rawcommabs.midStr("server_time=\"", "\"", false).toUInt());

    auto ag = new nicolive::GetCommPostKey(livewaku->getThread(), lastBlockNum, userSession, this);
    connect(ag, &nicolive::GetCommPostKey::got, this, [&](QString postKey){
      this->postKey = postKey;
    });
    ag->get();

    // set timer to get post_key
    postkeyTimer.start(10000);

    return;
  }

  if (rawcomm.startsWith("<chat_result")) {
    const auto status = rawcommabs.midStr("status=\"", "\"", false);

    if (status != "0") {
      emit error("CommentConnection::readOneRawComment", status);
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

    auto ag = new nicolive::GetCommPostKey(livewaku->getThread(), block, userSession, this);
    connect(ag, &nicolive::GetCommPostKey::got, this, [&](QString postKey){
      this->postKey = postKey;
    });
    ag->get();
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

void CommentConnection::getPostKey(QString userSession)
{
  auto ag = new nicolive::GetCommPostKey(livewaku->getThread(), lastBlockNum,
                                     userSession, this);
  connect(ag, &nicolive::GetCommPostKey::got, this, [&](QString postKey){
    this->postKey = postKey;
  });
  ag->get();
}

}
