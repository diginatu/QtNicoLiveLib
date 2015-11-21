#include "testclass.h"

TestClass::TestClass(QObject *parent) : QObject(parent)
{
}

void TestClass::fetch()
{
  QTextStream qtin(stdin);
  QString userSession, broadID;

  qDebug() << "input user session : ";
  qtin >> userSession;
  qDebug() << "input broadcast ID : ";
  qtin >> broadID;

  // create LiveWaku, which deals a broadcast
  nicolive::LiveWaku* livewaku = new nicolive::LiveWaku(broadID, this);

  // comment connection
  auto connection = new nicolive::CommentConnection(livewaku, userSession, livewaku);

  connect(livewaku, &nicolive::LiveWaku::gotInfornation, this, [=](){
    // livewaku has information about the broadcast when it gets
    QTextStream out(stdout);
    out << "title : " << livewaku->getTitle() << "\n" << endl;

    // connect comment server after getting information
    connection->doConnect();
  });
  connect(livewaku, &nicolive::LiveWaku::gotInfornationError,
          this, [](QString error){
    qDebug() << "getInformation error : " << error;
    QCoreApplication::quit();
  });

  connect(connection, &nicolive::CommentConnection::newComment,
          this, [](int num, bool premium, QString userID, QString comment,
                  QDateTime commentTime, bool iyayo, bool broadcaster){
    (void)premium, (void)userID, (void)commentTime, (void)iyayo, (void)broadcaster;
    QTextStream out(stdout);
    out << num << " " << comment << endl;
  });
  connect(connection, &nicolive::CommentConnection::error,
          this, [](QString position, QString error){
    qDebug() << position << " : " << error;
    QCoreApplication::quit();
  });

  // Finally, call PlayerStatus internally and get information in it
  livewaku->getInformation(userSession);
}
