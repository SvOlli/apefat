/**
 * PsmkPatternSequenceDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPATTERNSEQUENCEDIALOG_HPP
#define PSMKPATTERNSEQUENCEDIALOG_HPP PSMKPATTERNSEQUENCEDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */
#include <QList>
#include <QString>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QGridLayout;
class QPushButton;
class QScrollArea;
class QWidget;

/* forward declaration of local classes */
class PsmkPacker;
class PsmkPatternSequenceEditor;
class PsmkPatternWidget;

/*!
 \brief \todo

*/
class PsmkPatternSequenceDialog : public QDialog
{
   Q_OBJECT

public:
   explicit PsmkPatternSequenceDialog( const QList<PsmkPatternWidget*> &voice0,
                                       const QList<PsmkPatternWidget*> &voice1,
                                       const QList<bool> &hihat,
                                       const PsmkPacker *psmkPacker,
                                       QWidget *parent = 0 );
   virtual ~PsmkPatternSequenceDialog();

   QList<PsmkPatternWidget*> getVoicePatterns( int voice ) const;
   QList<bool> getHiHat() const;

public slots:
   void setTexts();

private:
   Q_DISABLE_COPY( PsmkPatternSequenceDialog )

   QScrollArea                *mpScrollArea;
   PsmkPatternSequenceEditor  *mpEditor;
   QPushButton                *mpCancel;
   QPushButton                *mpOk;
};

#endif
