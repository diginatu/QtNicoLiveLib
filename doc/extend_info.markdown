ExtendInfo Class
================

ExtendInfo はニコ生放送の延長情報を取得するクラスです。

`Header: #include <nl/extendinfo.h>`

Public Functions
----------------

### `explicit ExtendInfo(QObject* parent = 0);`

コンストラクタ

### `void get(const QString& broadID, const QString& userSession);`

リクエストを発行します。
取得後、Signalのgotまたはerrorをemitします。

Signals
-------

### `void error(QString code, QString description);`

エラーになった場合にエミットされるシグナルです。

参考: [get](#void-getconst-qstring-broadid-const-qstring-usersession)

### `void got(QVector< QVector<QString> > extendList);`

取得成功した場合にエミットされるシグナルです。

extendListはニコ生API名で順に
`label, code, item, num, price`
の入ったQVectorが入ったQVectorです。

参考: [get](#void-getconst-qstring-broadid-const-qstring-usersession)
