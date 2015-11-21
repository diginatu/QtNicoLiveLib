#ifndef COMMENTCONNECTION_H
#define COMMENTCONNECTION_H

#include <QObject>
#include <QTimer>
#include "livewaku.h"
#include "getcommpostkey.h"
#include "strabstractor.h"
#include "httpgetter.h"

namespace nicolive {

class CommentConnection : public QObject
{
  Q_OBJECT
public:
  explicit CommentConnection(LiveWaku* livewaku,
                             const QString& userSession, QObject *parent = 0);
  ~CommentConnection();

  void doConnect();
  void close();
  void sendComment(QString text, bool iyayo);

  QDateTime getOpenTime() const;

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
  bool isConnected();

private slots:
  void sendNull();
  void getPostKey(QString userSession);
};

}

#endif // COMMENTCONNECTION_H
