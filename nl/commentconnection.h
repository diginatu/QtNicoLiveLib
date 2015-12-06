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
/*!
 * コネクションの保持のためのNULL送信や、
 * コメント送信のためのポストキー取得が自動で行われます。
 *
 * LiveWaku を作成しコメント受信を行う例
 * \include LiveComment/testclass.cpp
 */

class CommentConnection : public QObject
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit CommentConnection(LiveWaku* livewaku,
                             const QString& userSession, QObject *parent = 0);
  ~CommentConnection();

  //! 接続した後は connectionStarted(), connectionClosed(), newComment()
  //! がemitされます。
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

private:
  QTcpSocket* socket;
  nicolive::LiveWaku* livewaku;
  QTimer nullDataTimer, postkeyTimer;
  int lastBlockNum;
  QByteArray lastRawComm;
  QString userSession;
  QString ticket;
  QDateTime openTime, serverTime;

signals:
  //! エラーになった場合にエミットされるシグナル。
  /*!
   * \param errorPosition エラー箇所、関数名
   * \param code エラーメッセージ
   */
  void error(QString errorPosition, QString code);
  //! 接続が開始した時にエミットされるシグナル。
  void connectionStarted();
  //! 接続が切断した時にエミットされるシグナル。
  void connectionClosed();
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
};

}

#endif // COMMENTCONNECTION_H
