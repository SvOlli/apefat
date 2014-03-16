/**
 * PsmkClipboard.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKCLIPBOARD_HPP
#define PSMKCLIPBOARD_HPP PSMKCLIPBOARD_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QString>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLabel;
class QTextEdit;

/* forward declaration of local classes */

/*!
 \brief \todo

*/
class PsmkClipboard : public QGroupBox
{
   Q_OBJECT

public:
   explicit PsmkClipboard( QWidget *parent = 0 );
   virtual ~PsmkClipboard();

public slots:
   void parseClipboardData();
   void setTexts();

private:
   Q_DISABLE_COPY( PsmkClipboard )

   QLabel      *mpMessage1;
   QTextEdit   *mpMessage2;
};

#endif
