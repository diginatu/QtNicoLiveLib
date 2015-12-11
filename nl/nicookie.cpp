﻿/*
 * Nicookie.cpp ニコニコ動画ユーザセッションクッキー取得ライブラリ for Qt
 */
#include <QProcessEnvironment>
#include <QFile>
#include <QTemporaryFile>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QIODevice>

// for Firefox and Chrome
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

// for Safari
#ifdef Q_OS_OSX
#include <QDataStream>
#endif

// for IE
#ifdef Q_OS_WIN
#include <windows.h>
#include <wininet.h>
#ifdef _MSC_VER
#include <iepmapi.h>
#endif // _MSC_VER
#endif

// for Chrome
#ifdef Q_OS_WIN
#include <wincrypt.h>
#else // Q_OS_WIN
#ifdef Q_OS_OSX
#include <Security/Security.h>
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif // Q_OS_OSX
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#endif // Q_OS_WIN

#include "nicookie.h"

namespace nicolive {

const QString Nicookie::COOKIE_URL = "http://www.nicovideo.jp/";
const QString Nicookie::COOKIE_HOST = ".nicovideo.jp";
const QString Nicookie::COOKIE_NAME = "user_session";
const QString Nicookie::COOKIE_PATH = "/";

const QString Nicookie::INTERNET_EXPLORER = "Internet Explorer";
const QString Nicookie::SAFARI = "Safari";
const QString Nicookie::FIREFOX = "Mozilla Firefox";
const QString Nicookie::CHROME = "Google Chrome";
const QString Nicookie::OPERA = "Opera";

const QStringList Nicookie::BROWSER_LIST = {
  #ifdef Q_OS_WIN
  Nicookie::INTERNET_EXPLORER,
  #endif // Q_OS_WIN
  #ifdef Q_OS_MAC
  Nicookie::SAFARI,
  #endif // Q_OS_MAC
  Nicookie::FIREFOX,
  Nicookie::CHROME,
  //    Nicookie::OPERA,
};

Nicookie::Nicookie(QObject *parent) : QObject(parent)
{
  this->errorNum = Nicookie::NoError;
  this->userSession = QString();
}

Nicookie::~Nicookie()
{

}

QString Nicookie::getUserSession(QString browser)
{
  clear();
  if (browser == Nicookie::INTERNET_EXPLORER) {
#ifdef Q_OS_WIN
    findInternetExplorer();
#else
    setError(Nicookie::NotSupportBrowserError);
#endif // Q_OS_WIN
  } else if (browser == Nicookie::SAFARI) {
#ifdef Q_OS_OSX
    findSafari();
#else
    setError(Nicookie::NotSupportBrowserError);
#endif // Q_OS_OSX
  } else if (browser == Nicookie::FIREFOX) {
    findFirefox();
  } else if (browser == Nicookie::CHROME) {
    findChrome();
  } else if (browser == Nicookie::OPERA) {
    findOpera();
  } else {
    setError(Nicookie::UnknownBrowserError);
  }
  return this->userSession;
}

const QStringList &Nicookie::getBrowserList() const
{
  return Nicookie::BROWSER_LIST;
}

Nicookie::Error Nicookie::error() const
{
  return this->errorNum;
}

const QString Nicookie::errorString() const
{
  switch(this->errorNum) {
  case Nicookie::NoError:
    return QStringLiteral("エラーはありません。");
    break;
  case Nicookie::NotImplementError:
    return QStringLiteral("まだ、実装していません。");
    break;
  case Nicookie::UnknownBrowserError:
    return QStringLiteral("不明なブラウザです。");
    break;
  case Nicookie::NotSupportBrowserError:
    return QStringLiteral("未対応のブラウザです。");
    break;
  case Nicookie::NotFoundDataError:
    return QStringLiteral("データが見つかりませんでした。");
    break;
  case Nicookie::NotFoundCookiesFileError:
    return QStringLiteral("クッキーファイルが見つかりませんでした。");
    break;
  case Nicookie::InvalidDataFormtaError:
    return QStringLiteral("データフォーマットが不正です。");
    break;
  case Nicookie::FailedDecrytError:
    return QStringLiteral("復号に失敗しました。");
    break;
  case Nicookie::FailedOpenCookiesFileError:
    return QStringLiteral("クッキーファイルを開けませんでした。");
    break;
  case Nicookie::FailedReadDataError:
    return QStringLiteral("データを読み込めませんでした。");
    break;
  case Nicookie::SQLiteError:
    return QStringLiteral("SQLite3の処理でエラーが発生しました。");
    break;
  case Nicookie::FailedParseProfileError:
    return QStringLiteral("プロファイルの解析でエラーが発生しました。");
    break;
  default:
    return QStringLiteral("不明なエラーです。");
  }
}

bool Nicookie::hasError() const
{
  return this->errorNum != Nicookie::NoError;
}

void Nicookie::clear()
{
  setError(Nicookie::NoError);
  this->userSession = QString();
}

#ifdef Q_OS_WIN
// # Windows #

// ## Internet Explorer ##
bool Nicookie::findInternetExplorer()
{
  WCHAR cookie_data[256];
  DWORD cookie_data_size = 256;
  BOOL result = false;
#ifdef _MSC_VER
  // 保護モードでの取得。VisualStudioでのみ有効。
  // 32bitは32bitだけ、64bitは64bitだけしか取得できない。
  HRESULT hr;
  hr = IEGetProtectedModeCookie(Nicookie::COOKIE_URL.toStdWString().c_str(),
                                Nicookie::COOKIE_NAME.toStdWString().c_str(),
                                cookie_data,
                                &cookie_data_size,
                                0);
  result = (hr == S_OK);
#endif // _MSC_VER
  if (!result) {
    result = InternetGetCookieW(Nicookie::COOKIE_URL.toStdWString().c_str(),
                                Nicookie::COOKIE_NAME.toStdWString().c_str(),
                                cookie_data,
                                &cookie_data_size);
  }
  if (result) {
    this->userSession = QString::fromStdWString(std::wstring(cookie_data));
    return true;
  } else {
    setError(Nicookie::NotFoundDataError);
    return false;
  }
}
#endif // Q_OS_WIN

#ifdef Q_OS_OSX
// # Mac OSX #

// ## Safari ##
bool Nicookie::findSafari()
{
  QString cookies_path;
  cookies_path += QProcessEnvironment::systemEnvironment().value("HOME");
  cookies_path += "/Library/Cookies/Cookies.binarycookies";

  QFile cookies_file(cookies_path);
  if (!cookies_file.exists()) {
    setError(Nicookie::NotFoundCookiesFileError);
    return false;
  }

  if (!cookies_file.open(QIODevice::ReadOnly)) {
    setError(Nicookie::FailedOpenCookiesFileError);
    return false;
  }

  bool result = safariFindFile(cookies_file);
  cookies_file.close();
  return result;
}
bool Nicookie::safariFindFile(QIODevice &device) {
  // Signature
  QByteArray headr_signature("cook", 4);
  if (headr_signature != device.read(4)) {
    setError(Nicookie::InvalidDataFormtaError);
    return false;
  }

  // No. of pages
  quint32 page_num = readUint32BE(device);
  if (page_num == 0) {
    setError(Nicookie::NotFoundDataError);
    return false;
  }

  // Page N Size
  QList<quint32> page_size_list;
  for (quint32 i = 0; i < page_num; ++i) {
    page_size_list.append(readUint32BE(device));
  }
  if (device.atEnd()) {
    setError(Nicookie::InvalidDataFormtaError);
    return false;
  }

  // Page N
  for (auto &page_size: page_size_list) {
    qint64 pos = device.pos();
    if (safariFindPage(device)) return true;
    device.seek(pos + page_size);
  }

  if (!hasError()) {
    setError(Nicookie::NotFoundDataError);
  }
  return false;
}

bool Nicookie::safariFindPage(QIODevice &device)
{
  qint64 begin_pos = device.pos();

  // Page Header
  quint32 page_header = readUint32BE(device);
  if (page_header != 0x00000100) {
    setError(Nicookie::InvalidDataFormtaError);
    return false;
  }

  // No. of cookies
  quint32 cookie_num = readUint32LE(device);
  if (cookie_num == 0) {
    // エラーじゃ無い？
    return false;
  }

  // Cookie N offset
  QList<quint32> cookie_offset_list;
  for (quint32 i = 0; i < cookie_num; i++) {
    cookie_offset_list.append(readUint32LE(device));
  }

  // Cookie N
  for (auto &cookie_offset: cookie_offset_list) {
    device.seek(begin_pos + cookie_offset);
    if (safariFindCookie(device)) return true;
  }

  return false;
}

bool Nicookie::safariFindCookie(QIODevice &device)
{
  qint64 begin_pos = device.pos();

  readUint32LE(device); // cookie_size not use
  readUint32LE(device); // unknown
  readUint32LE(device); // flags not use
  readUint32LE(device);  // unknown
  quint32 url_offset = readUint32LE(device);
  quint32 name_offset = readUint32LE(device);
  quint32 path_offset = readUint32LE(device);
  quint32 value_offset = readUint32LE(device);

  // check url
  device.seek(begin_pos + url_offset);
  if (!checkSameStr(device, Nicookie::COOKIE_HOST)) return false;

  // check name
  device.seek(begin_pos + name_offset);
  if (!checkSameStr(device, Nicookie::COOKIE_NAME)) return false;

  // check path
  device.seek(begin_pos + path_offset);
  if (!checkSameStr(device, Nicookie::COOKIE_PATH)) return false;

  device.seek(begin_pos + value_offset);
  QString str = readStr(device);
  if (str.isEmpty()) {
    setError(Nicookie::NotFoundDataError);
    return false;
  } else {
    this->userSession = str;
    return true;
  }
}
#endif // Q_OS_OSX

// # Windows and Mac OSX and Linux #

// ## Firefox ##
bool Nicookie::findFirefox()
{
  QString profile_ini;
#if defined(Q_OS_WIN)
  profile_ini += QProcessEnvironment::systemEnvironment().value("APPDATA");
  profile_ini += "\\Mozilla\\Firefox\\profiles.ini";
#elif defined(Q_OS_OSX)
  profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
  profile_ini += "/Library/Application Support/Firefox/profiles.ini";
#else
  profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
  profile_ini += "/.mozilla/firefox/profiles.ini";
#endif
  QStringList profile_list = firefoxGetProfileList(profile_ini);
  if (profile_list.isEmpty()) return false;
  for (auto &profile_path: profile_list) {
    if (firefoxFindValue(QDir(profile_path).filePath("cookies.sqlite"))) {
      return true;
    }
  }
  return false;
}

QStringList Nicookie::firefoxGetProfileList(const QString &profile_ini)
{
  QStringList list;
  QFile profile_file(profile_ini);
  if (!profile_file.exists()) {
    setError(Nicookie::FailedParseProfileError);
    return list;
  }

  QTemporaryFile profile_temp;
  if (!profile_temp.open()) {
    setError(Nicookie::FailedParseProfileError);
    return list;
  }
  if (!profile_file.open(QIODevice::ReadOnly)) {
    setError(Nicookie::FailedParseProfileError);
    profile_temp.close();
    return list;
  }
  profile_temp.write(profile_file.readAll());
  profile_file.close();
  profile_temp.close();

  QSettings profile_settings(profile_temp.fileName(), QSettings::IniFormat);
  if (profile_settings.status() != QSettings::NoError) {
    setError(Nicookie::FailedParseProfileError);
    return list;
  }

  for (auto &group: profile_settings.childGroups()) {
    if (group.startsWith("Profile")) {
      profile_settings.beginGroup(group);
      QString path;
      if (profile_settings.value("IsRelative", 1).toInt() == 1) {
        path = QFileInfo(profile_file).dir().filePath(
              profile_settings.value("Path").toString());
      } else {
        path = profile_settings.value("Path").toString();
      }
      if (profile_settings.value("Default", 0).toInt() == 1) {
        list.prepend(path);
      } else {
        list.append(path);
      }
      profile_settings.endGroup();
    }
  }

  if (list.isEmpty()) {
    setError(Nicookie::FailedParseProfileError);
  }
  return list;
}

bool Nicookie::firefoxFindValue(const QString &cookies_path)
{
  QString query = "SELECT value FROM moz_cookies WHERE "
                  "host = :host AND "
                  "name = :name AND "
                  "path = :path;";
  QMap<QString, QVariant> placeholders;
  placeholders[":host"] = Nicookie::COOKIE_HOST;
  placeholders[":name"] = Nicookie::COOKIE_NAME;
  placeholders[":path"] = Nicookie::COOKIE_PATH;
  QMap<QString, QVariant> values;
  values["value"] = QVariant();

  if (!querySqlite3(cookies_path, query, placeholders, values)) {
    return false;
  }

  this->userSession = values["value"].toString();
  return true;
}

bool Nicookie::findChrome()
{
  QString cookies_path;
#if defined(Q_OS_WIN)
  cookies_path += QProcessEnvironment::systemEnvironment().value("LOCALAPPDATA");
  cookies_path += "\\Google\\Chrome\\User Data\\Default\\Cookies";
#elif defined(Q_OS_OSX)
  cookies_path += QProcessEnvironment::systemEnvironment().value("HOME");
  cookies_path +=
      "/Library/Application Support/Google/Chrome/Default/Cookies";
#else
  cookies_path += QProcessEnvironment::systemEnvironment().value("HOME");
  cookies_path += "/.config/chromium/Default/Cookies";
#endif
  return chromeFindValue(cookies_path);
}

bool Nicookie::chromeFindValue(const QString &cookies_path)
{
  QString query = "SELECT value,encrypted_value FROM cookies WHERE "
                  "host_key = :host AND "
                  "name = :name AND "
                  "path = :path;";
  QMap<QString, QVariant> placeholders;
  placeholders[":host"] = Nicookie::COOKIE_HOST;
  placeholders[":name"] = Nicookie::COOKIE_NAME;
  placeholders[":path"] = Nicookie::COOKIE_PATH;
  QMap<QString, QVariant> values;
  values["value"] = QVariant();
  values["encrypted_value"] = QVariant();

  if (!querySqlite3(cookies_path, query, placeholders, values)) {
    return false;
  }

  this->userSession = values["value"].toString();
  if (this->userSession.isEmpty()) {
    this->userSession = chromeDecrypt(
          values["encrypted_value"].toByteArray());
  }
  return !this->userSession.isEmpty();
}

QString Nicookie::chromeDecrypt(const QByteArray &encrypt_data)
{
  QString data;
#ifdef Q_OS_WIN
  DATA_BLOB encrypt_data_blob;
  encrypt_data_blob.pbData = (BYTE*)(encrypt_data.data());
  encrypt_data_blob.cbData = static_cast<DWORD>(encrypt_data.size());
  DATA_BLOB plain_data_blob;
  BOOL result = CryptUnprotectData(&encrypt_data_blob,
                                   NULL, NULL, NULL, NULL, 0,
                                   &plain_data_blob);
  if (!result) {
    setError(Nicookie::FailedDecrytError);
    return QString();
  }
  data = (QByteArray((char *)(plain_data_blob.pbData),
                     plain_data_blob.cbData));
  LocalFree(plain_data_blob.pbData);
#else // O_QS_WIN

#ifdef Q_OS_OSX
  // https://developer.apple.com/library/mac/documentation/Security/Reference/keychainservices/index.html#//apple_ref/c/func/SecKeychainFindGenericPassword
  UInt32 password_size = 0;
  void *password = NULL;
  OSStatus os_status;
  os_status = SecKeychainFindGenericPassword(NULL,
                                             19, "Chrome Safe Storage",
                                             6, "Chrome",
                                             &password_size, &password,
                                             NULL);
  if (password_size == 0) {
    setError(Nicookie::FailedDecrytError);
    SecKeychainItemFreeContent(NULL, password);
    return data;
  }
#else // Q_OS_OSX
  int password_size = 7;
  void *password = (void *)"peanuts";
#endif // Q_OS_OSX

  const int enc_key_size = 16;
  unsigned char enc_key[enc_key_size];

#ifdef Q_OS_OSX
  int iterations = 1003;
#else // Q_OS_OSX
  int iterations = 1;
#endif // Q_OS_OSX

  const char *salt = "saltysalt";
  int pbkdf2_r = PKCS5_PBKDF2_HMAC_SHA1((char *)password, password_size,
                                        (unsigned char *)salt, strlen(salt),
                                        iterations,
                                        enc_key_size, enc_key);
  if (!pbkdf2_r) {
    setError(Nicookie::FailedDecrytError);
#ifdef Q_OS_OSX
    SecKeychainItemFreeContent(NULL, password);
#endif // Q_OS_OSX
    return data;
  }

  const int iv_size = 16;
  unsigned char iv[iv_size];
  for (int i = 0; i < iv_size; i++) iv[i] = ' ';

  // alwayes enc size >= dec size
  int plain_value_size = encrypt_data.size();
  char *plain_value = (char *)malloc(plain_value_size);
  if (plain_value == NULL) {
    setError(Nicookie::FailedDecrytError);
#ifdef Q_OS_OSX
    SecKeychainItemFreeContent(NULL, password);
#endif // Q_OS_OSX
    return data;
  }

  int result = 1;
  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);

  result = EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, enc_key, iv);
  if (!result) {
    setError(Nicookie::FailedDecrytError);
    EVP_CIPHER_CTX_cleanup(&ctx);
    free(plain_value);
#ifdef Q_OS_OSX
    SecKeychainItemFreeContent(NULL, password);
#endif // Q_OS_OSX
    return data;
  }

  result = EVP_DecryptUpdate(&ctx,
                             (unsigned char *)plain_value,
                             &plain_value_size,
                             (unsigned char *)(encrypt_data.data() + 3),
                             encrypt_data.size() - 3);
  if (!result) {
    setError(Nicookie::FailedDecrytError);
    EVP_CIPHER_CTX_cleanup(&ctx);
    free(plain_value);
#ifdef Q_OS_OSX
    SecKeychainItemFreeContent(NULL, password);
#endif // Q_OS_OSX
    return data;
  }

  int fin_size = 0;
  result = EVP_DecryptFinal_ex(&ctx,
                               (unsigned char *)(plain_value +
                                                 plain_value_size),
                               &fin_size);
  if (!result) {
    setError(Nicookie::FailedDecrytError);
    EVP_CIPHER_CTX_cleanup(&ctx);
    free(plain_value);
#ifdef Q_OS_OSX
    SecKeychainItemFreeContent(NULL, password);
#endif // Q_OS_OSX
    return data;
  }

  EVP_CIPHER_CTX_cleanup(&ctx);

  plain_value[plain_value_size + fin_size] = '\0';
  data = plain_value;

  free(plain_value);
#ifdef Q_OS_OSX
  SecKeychainItemFreeContent(NULL, password);
#endif // Q_OS_OSX
#endif // O_QS_WIN
  return data;
}

bool Nicookie::findOpera()
{
  setError(Nicookie::NotImplementError);
  return false;
}

void Nicookie::setError(Error num)
{
  this->errorNum = num;
}

bool Nicookie::querySqlite3(const QString &sqlite3_file,
                            const QString &query,
                            const QMap<QString, QVariant> &placeholders,
                            QMap<QString, QVariant> &values)
{
  bool result = false;
  do {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",
                                                "nicookie_sqlite3");
    if (!db.isValid()) {
      setError(Nicookie::SQLiteError);
      break;
    }

    db.setDatabaseName(sqlite3_file);
    if (!db.open()) {
      setError(Nicookie::SQLiteError);
      db.close();
      break;
    }

    QSqlQuery sql_query(db);
    if (!sql_query.prepare(query)) {
      setError(Nicookie::SQLiteError);
      db.close();
      break;
    }

    for (auto &name: placeholders.keys()) {
      sql_query.bindValue(name, placeholders[name]);
    }
    if (!sql_query.exec()) {
      setError(Nicookie::SQLiteError);
      sql_query.finish();
      db.close();
      break;
    }

    if (!sql_query.first()) {
      setError(Nicookie::NotFoundDataError);
      sql_query.finish();
      db.close();
      break;
    }

    for (auto &name: values.keys()) {
      values[name] = sql_query.value(name);
    }
    sql_query.finish();
    db.close();
    result = true;
  } while(false);
  QSqlDatabase::removeDatabase("nicookie_sqlite3");
  return result;
}

#ifdef Q_OS_OSX
quint32 Nicookie::readUint32BE(QIODevice &device)
{
  QDataStream stream(device.read(4));
  stream.setByteOrder(QDataStream::BigEndian);
  quint32 i;
  stream >> i;
  return i;
}

quint32 Nicookie::readUint32LE(QIODevice &device)
{
  QDataStream stream(device.read(4));
  stream.setByteOrder(QDataStream::LittleEndian);
  quint32 i;
  stream >> i;
  return i;
}
#endif // Q_OS_OSX

bool Nicookie::checkSameStr(QIODevice &device, const QString &str)
{
  char input_c;
  for (auto &c: str) {
    if (!device.getChar(&input_c)) {
      setError(Nicookie::FailedReadDataError);
      return false;
    }
    if (c != input_c) return false;
  }
  if (!device.getChar(&input_c)) {
    setError(Nicookie::FailedReadDataError);
    return false;
  }
  if (input_c != '\0') return false;
  return true;
}

QString Nicookie::readStr(QIODevice &device)
{
  QString str;
  while (true) {
    char input_c;
    if (!device.getChar(&input_c)) {
      setError(Nicookie::FailedReadDataError);
      return QString();
    }
    if (input_c == '\0') {
      break;
    } else {
      str.append(input_c);
    }
  }
  return str;
}

}
