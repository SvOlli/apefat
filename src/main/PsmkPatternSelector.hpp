/**
 * PsmkPatternSelector.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKPATTERNSELECTOR_HPP
#define PSMKPATTERNSELECTOR_HPP PSMKPATTERNSELECTOR_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QVariant>

/* local library headers */

/* local headers */
#include "PsmkConfig.hpp"

/* forward declaration of Qt classes */
class QGridLayout;
class QLabel;
class QPushButton;
class QStackedWidget;
class QTimer;
class QToolButton;

/* forward declaration of local classes */

/*!
 \brief constants set for psmk tracks

*/
class PsmkPatternSelector : public QWidget
{
   Q_OBJECT

   friend class PsmkPacker;

public:
   explicit PsmkPatternSelector( QWidget *parent = 0 );
   virtual ~PsmkPatternSelector();

   QVariantList toVariantList() const;
   bool fromVariantList( const QVariantList &variantList );

   void setInstrumentCache( const QByteArray &instruments );

public slots:
   void setTexts();

   void setInstrument( int index, quint8 value );
   void setInstruments( quint8 tones[8] );
   void setPattern( int pattern );
   void moveFirst();
   void movePrevious();
   void moveNext();
   void moveLast();
   void insertBefore();
   void insertAfter();
   void runPSE();

signals:
   void changed();

private:
   Q_DISABLE_COPY( PsmkPatternSelector )

   void setup();
   void clear();
   void insert( int pos );

   QTimer                  *mpUpdateDelay;
   QLabel                  *mpPositionText; /*!< \brief info about current voice and bar */
   QToolButton             *mpFirstButton; /*!< \brief button for going to first bar */
   QToolButton             *mpPreviousButton; /*!< \brief button for going to last bar */
   QToolButton             *mpNextButton; /*!< \brief button for going to next bar */
   QToolButton             *mpLastButton; /*!< \brief button for going to last bar */
   QToolButton             *mpAddBeforeButton; /*!< \brief button for adding before current bar */
   QToolButton             *mpAddAfterButton; /*!< \brief button for adding after current bar */
   QStackedWidget          *mpVoice0Stack;
   QStackedWidget          *mpVoice1Stack;
   QStackedWidget          *mpHiHatStack;
   QPushButton             *mpPSE;
   quint8                  mInstrumentCache[PsmkConfig::InstrumentsInSong];
};

#endif
