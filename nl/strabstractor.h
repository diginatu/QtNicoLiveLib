#ifndef STRABSTRACTOR_H
#define STRABSTRACTOR_H

#include <QObject>

namespace nicolive {

class StrAbstractor : public QObject
{
	Q_OBJECT
public:
  explicit StrAbstractor(const QString& doc, QObject* parent = 0);

  //! Return the QString between "start" and "end".
  /*!
   * Return null QString if any matched substring are not found.
   * Set current position to the next one of "end" found if foward is true (default).
   * If start is empty string, the returned value starts from begining of doc.
   * If end do so, it will search to the end of the doc.
   */
  QString midStr(const QString& start, const QString& end, bool foward = true);

  //! Return the pointer to new nicolive::StrAbstractor which contains the substring found by midStr.
  /*! Return nullptr if any matched substring are not found. */
  StrAbstractor* mid(const QString& start, const QString& end, bool foward = true);

  //! forward position to the position that st found.
  /*! return -1 if no st found, otherwise return the position that st found. */
  int forward(const QString& st);

  //! Set current position, midStr searches from here, to "pos".
  void setPosition(int po);
  //! Relative version of setPosition()
  void setRelativePosition(int pos);
  //! Return position searches starts from
  int getPosition();
  //! Return whole document
  QString toString();

private:
	int pos;
  QString doc;

};

}

#endif // STRABSTRACTOR_H
