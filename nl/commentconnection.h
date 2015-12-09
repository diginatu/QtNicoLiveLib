#ifndef COMMENTCONNECTION_H
#define COMMENTCONNECTION_H

#include <QObject>
#include <QTimer>
#include "qtnicolivelib.h"
#include "livewaku.h"
#include "getcommpostkey.h"
#include "strabstractor.h"
#include "httpgetter.h"

namespace nicolive {

//! ニコ生のコメントの接続を管理するクラス。
/*!
 * コネクションの保持のためのNULL送信や、
 * コメント送信のためのポストキー取得が自動で行われます。
 *
 * LiveWaku を作成しコメント受信を行う例
 * \include LiveComment/testclass.cpp
 */

class QTNICOLIVELIBSHARED_EXPORT CommentConnection : public QObject
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit CommentConnection(LiveWaku* livewaku,
                             const QString& userSession, QObject *parent = 0);
  ~CommentConnection();

  //! コメントサーバに接続します。
  /*!
   * 接続した後は connectionStarted(), connectionClosed(), newComment() がemitされます。
   */
  void doConnect();
  //! 接続を切断します。
  void close();
  //! 通常のコメントを送信します。
  /*!
   * \param iyayo 184コメント
   */
  void sendComment(QString text, bool iyayo = false);

  //! 接続開始した時刻を返します。
  QDateTime getOpenTime() const;
  //! 接続されているかどうかをboolで返します。
  bool isConnected();

  //! 再接続する試行回数を設定します。
  /*!
   * コメント送信が失敗した場合は再送信はされません。
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
private:
  QTcpSocket* socket;
  nicolive::LiveWaku* livewaku;
  QTimer* nullDataTimer;
  QTimer* postkeyTimer;
  int lastBlockNum;
  QByteArray lastRawComm;
  QString userSession;
  QString ticket;
  QDateTime openTime, serverTime;

  int reconnectTimes, reconnectN;
  int reconnectWaitMsec;
signals:
  //! エラーになった場合にエミットされるシグナル。
  /*!
   * \param errorPosition エラー箇所、関数名
   * \param code エラーメッセージ
   */
  void error(QString errorPosition, QString code);
  //! 接続が開始した時にエミットされるシグナル。
  void socketConnected();
  //! 接続が切断した時にエミットされるシグナル。
  void socketDisconnected();
  //! コメントの送信が完了した時にエミットされるシグナル。
  void submittedComment();
  //! コメントを一つ受信した時にエミットされるシグナル。
  void newComment(int num, bool premium, QString userID, QString comment,
                  QDateTime commentTime, bool iyayo, bool broadcaster);

private slots:
  void sendNull();
  void connected();
  void disconnected();
  void readyRead();
  void readOneRawComment(const QString& rawcomm);
  void connectionErrorOccured();
};

}

#endif // COMMENTCONNECTION_H
