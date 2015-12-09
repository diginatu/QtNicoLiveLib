#ifndef LIVEWAKU_H
#define LIVEWAKU_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include "qtnicolivelib.h"
#include "getplayerstatus.h"
#include "getcommpostkey.h"

namespace nicolive {

//! １つの生放送の情報を保持するクラス。
/*!
 * CommentConnection
 */

class QTNICOLIVELIBSHARED_EXPORT LiveWaku : public QObject
{
  Q_OBJECT
public:
  //! コンストラクタ
  explicit LiveWaku(QObject *parent = 0);
  //! コンストラクタ
  /*!
   * broadID をだけ指定すれば残りの情報は fetchInformation() で得られます。
   */
  explicit LiveWaku(const QString& broadID, QObject *parent = 0);


  // setter and getter
  QString getTitle() const;
  void setTitle(const QString& value);
  QString getBroadID() const;
  void setBroadID(const QString& value);
  QString getCommunityID() const;
  void setCommunityID(const QString& value);
  QString getOwnerName() const;
  void setOwnerName(const QString& value);
  QString getBroadcastToken() const;

  QDateTime getStTime() const;
  void setStTime(const QDateTime& value);
  QDateTime getEdTime() const;
  void setEdTime(const QDateTime& value);

  QString getUserID() const;
  QString getOwnerID() const;
  bool getUserPremium() const;

  QString getPostKey() const;

  QString getAddr() const;
  QString getThread() const;
  qint16 getPort() const;

  bool getOwnerBroad() const;
  void setOwnerBroad(bool value);
  QString getOwnerCommentToken() const;

  //! PlayerStatus を使って放送枠に関する情報を取得します。
  /*!
   * CommentConnection で doConnect する前に実行する必要があります。
   *
   * 取得完了すると gotInfornationError() または gotInfornation() を emit します。
   */
  void fetchInformation(const QString& userSession);
  //! コメント送信に必要な PostKey の取得・更新をします。
  /*!
   * 取得完了すると gotInPostKeyError() または gotPostKey() を emit します。
   */
  void fetchPostKey(int lastBlockNum, const QString& userSession);

signals:
  //! fetchInformation() でエラーになった場合にエミットされるシグナルです。
  void gotInfornationError(QString code);
  //! fetchInformation() で取得成功した場合にエミットされるシグナルです。
  void gotInfornation();

  //! fetchPostKey() でエラーになった場合にエミットされるシグナルです。
  void gotPostKeyError();
  //! fetchPostKey() で取得成功した場合にエミットされるシグナルです。
  void gotPostKey();

private:
  QString title;
  QString broadID;
  QString communityID;
  QString ownerName;
  QString broadcastToken;

  QDateTime stTime;
  QDateTime edTime;

  QString userID, ownerID;
  bool userPremium;

  QString postKey;

  QString addr, thread;
  qint16 port;

  bool ownerBroad;
  QString ownerCommentToken;

};

}

#endif // LIVEWAKU_H
