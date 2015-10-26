﻿#include "httpgetter.h"

namespace nicolive {

HttpGetter::HttpGetter(QObject *parent) :
  QObject(parent),
  mManager(nullptr)
{
}

void HttpGetter::requestGet(const QNetworkRequest& rq)
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(got(QNetworkReply*)));

  mManager->get(rq);
}

void HttpGetter::requestPost(const QNetworkRequest& rq, QIODevice* data)
{
  if(mManager!=nullptr) delete mManager;
  mManager = new QNetworkAccessManager(this);

  connect(mManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(got(QNetworkReply*)));

  mManager->post(rq, data);
}

HttpGetter::~HttpGetter()
{
  if(mManager!=nullptr) delete mManager;
}

void HttpGetter::got(QNetworkReply* reply)
{
  reply->deleteLater();
  this->deleteLater();
}

}
