#ifndef ALERTCONNECTION_H
#define ALERTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDateTime>
#include <QProcess>
#include <QTimer>
#include "strabstractor.h"

namespace nicolive {

//! 生放送通知アラートの接続を管理するクラス。

class AlertConnection : public QObject
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit AlertConnection(const QString& domain, qint16 port,
                           const QString& thread, QObject *parent = 0);

  //! アラートに接続します。
  void doConnect();
  //! アラートの接続を切断します。
  void close();
  //! アラートが接続されているかどうかを bool で返します。
  bool isConnected();
  //! アラートに接続しているソケットを返します。
  QTcpSocket* getSocket() const;

  //! 再接続する試行回数を設定します。
  /*!
   * デフォルト値 3
   * \param time 待ち時間(msec)
   */
  void setReconnectTimes(int time);
  //! 再接続する際の待ち時間を設定します。
  /*!
   * デフォルト値 1000
   * \param time 待ち時間(msec)
   */
  void setReconnectWaitMsec(int time);
private slots:
  void connected();
  void disconnected();
  void readyRead();
  void readOneRawWaku(const QString& rawwaku);
  void connectionErrorOccured();

signals:
  void error(QString errorPosition, QString code);
  void gotNewWaku(QString broadID, QString CommunityID, QString nushiID);
  void socketConnected();
  void socketDisconnected();

private:
  QTcpSocket* socket;

  QString domain, thread;
  int port;

  QDateTime opentime;
  QByteArray lastRawWaku;
  QTimer* checkConnectionTimer;

  int reconnectTimes, reconnectN;
  int reconnectWaitMsec;
private slots:
  void checkConnection();
};

}

#endif // ALERTCONNECTION_H
