#include "strabstractor.h"

namespace nicolive {

StrAbstractor::StrAbstractor(const QString& doc, QObject* parent) :
  QObject(parent)
{
  pos = 0;
  this->doc = doc;
}

QString StrAbstractor::midStr(const QString& start, const QString& end, bool foward)
{
  int st;
  if (start.isEmpty()) st = pos;
  else                 st = doc.indexOf(start,pos);

  if ( st == -1 ) return QString();
  st += start.length();

  int ed;
  if (end.isEmpty()) ed = doc.length();
  else               ed = doc.indexOf(end,st);

  if ( ed == -1 ) return QString();

  if (foward) pos = ed + end.length();

  return doc.mid(st, ed-st);
}

StrAbstractor* nicolive::StrAbstractor::mid(const QString& start, const QString& end, bool foward)
{
  QString tmp = midStr(start, end, foward);
  if ( tmp.isNull() ) return nullptr;
  else return new nicolive::StrAbstractor(tmp, this);
}

int StrAbstractor::forward(const QString& st)
{
  int po = doc.indexOf(st,pos);
  if ( po == -1 ) return -1;

  pos = po;
  return po;
}

void StrAbstractor::setPosition(int pos)
{
  this->pos = pos;
}
void StrAbstractor::setRelativePosition(int pos)
{
  this->pos += pos;
}

int StrAbstractor::getPosition()
{
  return pos;
}

QString StrAbstractor::toString()
{
  return doc;
}

}
