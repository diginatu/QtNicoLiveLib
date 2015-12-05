#ifndef COMMENTCONNECTION_H
#define COMMENTCONNECTION_H

#include <QObject>
#include <QTimer>
#include "livewaku.h"
#include "getcommpostkey.h"
#include "strabstractor.h"
#include "httpgetter.h"

namespace nicolive {

//! ニコ生のコメント受信を行うクラス。

class CommentConnection : public QObject
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit CommentConnection(LiveWaku* livewaku,
                             const QString& userSession, QObject *parent = 0);
  ~CommentConnection();

  //! 接続した後はconnectionStarted(), connectionClosed(), newComment()がemitされます。
  void doConnect();
  //! 接続を切断します。
  void close();
  //! 通常のコメントを送信します。
  /*!
   * \arg iyayo 184コメント
   */
  void sendComment(QString text, bool iyayo = false);

  //! 接続開始した時刻を返します。
  QDateTime getOpenTime() const;
  //! 接続されているかどうかをboolで返します。
  bool isConnected();

private:
  QTcpSocket* socket;
  nicolive::LiveWaku* livewaku;
  QTimer nullDataTimer, postkeyTimer;
  int lastBlockNum;
  QString postKey;
  QByteArray lastRawComm;
  QString userSession;
  QString ticket;
  QDateTime openTime, serverTime;

signals:
  void error(QString errorPosition, QString code);
  void connectionStarted();
  void connectionClosed();
  void submittedComment();
  void newComment(int num, bool premium, QString userID, QString comment,
                  QDateTime commentTime, bool iyayo, bool broadcaster);

public slots:
  void connected();
  void disconnected();
  void readyRead();
  void readOneRawComment(const QString& rawcomm);

private slots:
  void sendNull();
  void getPostKey(QString userSession);
};

}

#endif // COMMENTCONNECTION_H
