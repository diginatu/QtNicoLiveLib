#include "alertconnection.h"

namespace nicolive {

AlertConnection::AlertConnection(const QString& addr, qint16 port,
                                 const QString& thread, QObject* parent):
  QObject(parent),
  socket(new QTcpSocket(this)),
  addr(addr),
  port(port),
  thread(thread),
  checkConnectionTimer(new QTimer(this))
{
  connect(socket, SIGNAL(connected()), this, SLOT(connected()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
          this, SLOT(connectionErrorOccured()));

  connect(checkConnectionTimer, SIGNAL(timeout()), this, SLOT(checkConnectionTimeout()));
}

AlertConnection::~AlertConnection()
{
  close();
}

void AlertConnection::doConnect()
{
  close();
  socket->connectToHost(addr, port);
}

void AlertConnection::close()
{
  socket->close();
}

bool AlertConnection::isConnected()
{
  return socket->state() != QAbstractSocket::UnconnectedState;
}

QTcpSocket* AlertConnection::getSocket() const
{
  return socket;
}

void AlertConnection::connected()
{
  emit socketConnected();

  QByteArray send;
  opentime = QDateTime::currentDateTime();

  send.append("<thread thread=\""+thread+"\" res_from=\"-1\" version=\"20061206\" />");
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

  checkConnectionTimer->start(61000);
}

void AlertConnection::disconnected()
{
  emit socketDisconnected();
  checkConnectionTimer->stop();
}

void AlertConnection::readyRead()
{
  const QList<QByteArray>& rawwakus( socket->readAll().split('\0') );
  if (rawwakus.isEmpty()) return;

  if (rawwakus.size() > 1) {
    emit readOneRawWaku(lastRawWaku + rawwakus[0]);
    for ( int i = 1; i < rawwakus.size()-1; ++i) {
      emit readOneRawWaku(rawwakus[i]);
    }
  }

  lastRawWaku = rawwakus.last();
}

void AlertConnection::readOneRawWaku(const QString& rawwaku)
{
  checkConnectionTimer->start();
  if (rawwaku.startsWith("<thread"))  return;

  StrAbstractor awaku(rawwaku);
  QString wakuS = awaku.midStr(">", "</chat>");

  QStringList wakur = wakuS.split(",");
  if(wakur.size() != 3)  return;

  const QString& broadID = wakur.at(0);
  const QString& communityID = wakur.at(1);
  const QString& nushiID = wakur.at(2);

  emit gotNewWaku(broadID, communityID, nushiID);
}

void AlertConnection::connectionErrorOccured()
{
  close();
  ++ reconnectN;
  if (reconnectN < reconnectTimes) {
    QTimer::singleShot(reconnectWaitMsec, Qt::VeryCoarseTimer, [=](){ doConnect(); });
  } else {
    emit error("QAbstractSocket::SocketError", socket->errorString());
  }
}

void AlertConnection::setReconnectWaitMsec(int time)
{
  reconnectWaitMsec = time;
}

void AlertConnection::setReconnectTimes(int time)
{
  reconnectTimes = time;
}

void AlertConnection::checkConnectionTimeout()
{
  close();
  doConnect();
}

}
