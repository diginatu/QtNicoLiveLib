#ifndef STRABSTRACTOR_H
#define STRABSTRACTOR_H

#include <QObject>

namespace nicolive {

class StrAbstractor : public QObject
{
	Q_OBJECT
public:
  explicit StrAbstractor(const QString& doc, QObject* parent = 0);
  QString midStr(const QString& start, const QString& end, bool foward = true);
  StrAbstractor* mid(const QString& start, const QString& end, bool foward = true);
  int forward(const QString& st);

	void setPosition(int po);
  void setRelativePosition(int pos);
  int getPosition();
  QString toString();

private:
	int pos;
  QString doc;

};

}

#endif // STRABSTRACTOR_H
