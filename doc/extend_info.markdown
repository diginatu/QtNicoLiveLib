ExtendInfo Class
================

ExtendInfo はニコ生放送の延長情報を取得するクラスです。

`Header: #include <nl/extendinfo.h>`

Member Struct
------

### ExtendItem

ニコ生APIで取得される延長情報に放送IDを追加した延長項目のクラスです。

```
QString label;
QString price;
QString num;
QString code;
QString item;
QString broadID;
```

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

### `void got(QVector<`[ExtendItem](#ExtendItem)`> extendList);`

取得成功した場合にエミットされるシグナルです。

参考: [get](#void-getconst-qstring-broadid-const-qstring-usersession)
