#ifndef STRABSTRACTOR_H
#define STRABSTRACTOR_H

#include <QObject>
#include "qtnicolivelib.h"

namespace nicolive {

//! Class for abstract keywords from a string (Internal Class)
class QTNICOLIVELIBSHARED_EXPORT StrAbstractor : public QObject
{
	Q_OBJECT
public:
  explicit StrAbstractor(const QString& doc, QObject* parent = 0);

  //! get QString between "start" and "end".
  /*!
   * Set current position to the next one of "end" found if foward is true (default).
   * If start is empty string, the returned value starts from begining of doc.
   * If end do so, it will search to the end of the doc.
   * \returns null QString if any matched substring are not found.
   */
  QString midStr(const QString& start, const QString& end, bool foward = true);

  //! make new nicolive::StrAbstractor pointer which contains the substring found by midStr.
  /*! \returns nullptr if any matched substring are not found. */
  StrAbstractor* mid(const QString& start, const QString& end, bool foward = true);

  //! forward position to the position that st found.
  /*! \returns -1 if no st found, otherwise returns the position that st found. */
  int forward(const QString& st);

  //! Set current position, midStr searches from here, to "pos".
  void setPosition(int po);
  //! Relative version of setPosition()
  void setRelativePosition(int pos);
  //! \returns position searches starts from
  int getPosition();
  //! \returns whole document
  QString toString();

private:
	int pos;
  QString doc;

};

}

#endif // STRABSTRACTOR_H
