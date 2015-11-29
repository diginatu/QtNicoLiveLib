Extend Class
============

Extend はニコ生放送の延長を実行するクラスです。

参考: [ExtendInfo Class](extend_info.markdown)

`Header: #include <nl/extend.h>`

Public Functions
----------------
  
### explicit Extend(QObject *parent = 0);

コンストラクタ

### get(const [ExtendInfo](extend_info.markdown)::[ExtendItem](extend_info.markdown#ExtendItem)& extendItem, const QString& broadcastToken, const QString& userSession);

リクエストを発行します。
取得後、Signalのgotまたはerrorをemitします。

[ExtendInfo Class](extend_info.markdown)でリストが取得できます。

Signals
-------

### void error(QString code, QString description);

エラーになった場合にエミットされるシグナルです。

参考: [get](#getconst-extendinfoextenditem-extenditem-const-qstring-broadcasttoken-const-qstring-usersession)

### void got();

取得成功した場合にエミットされるシグナルです。

参考: [get](#getconst-extendinfoextenditem-extenditem-const-qstring-broadcasttoken-const-qstring-usersession)
