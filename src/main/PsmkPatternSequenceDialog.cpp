/**
 * PsmkPatternSequenceDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPatternSequenceDialog.hpp"

/* system headers */

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextEdit>
#endif
#include <QClipboard>
#include <QMimeData>
#include <QSettings>


/* local library headers */

/* local headers */
#include "PsmkBeatWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkNoteWidget.hpp"
#include "PsmkPatternWidget.hpp"
#include "PsmkPatternProxyWidget.hpp"
#include "PsmkPatternSequenceEditor.hpp"


PsmkPatternSequenceDialog::PsmkPatternSequenceDialog( const QList<PsmkPatternWidget*> &voice0,
                                                      const QList<PsmkPatternWidget*> &voice1,
                                                      const QList<bool> &hihat,
                                                      const PsmkPacker *psmkPacker,
                                                      QWidget *parent )
: QDialog( parent )
, mpScrollArea( new QScrollArea( this ) )
, mpEditor( new PsmkPatternSequenceEditor( voice0, voice1, hihat, psmkPacker, this ) )
, mpCancel( new QPushButton( this ) )
, mpOk( new QPushButton( this ) )
{
#if 0
   if( parent )
   {
      QSize s( parent->size() );
      setMinimumSize( s );
   }
#endif
   mpScrollArea->setWidget( mpEditor );
   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setContentsMargins( 2, 2, 2, 2 );
   mainLayout->addWidget( mpScrollArea, 0, 0, 1, 2 );
   mainLayout->addWidget( mpCancel,     1, 0 );
   mainLayout->addWidget( mpOk,         1, 1 );

   connect( mpCancel, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( mpOk, SIGNAL(clicked()),
            this, SLOT(accept()) );

   setTexts();

   restoreGeometry( QSettings().value( "PatternSequenceDialogWindow" ).toByteArray() );
}


PsmkPatternSequenceDialog::~PsmkPatternSequenceDialog()
{
   QSettings().setValue( "PatternSequenceDialogWindow", saveGeometry() );
}

QList<PsmkPatternWidget *> PsmkPatternSequenceDialog::getVoicePatterns( int voice ) const
{
   return mpEditor->getVoicePatterns( voice );
}


QList<bool> PsmkPatternSequenceDialog::getHiHat() const
{
   return mpEditor->getHiHat();
}


void PsmkPatternSequenceDialog::setTexts()
{
   mpCancel->setText( tr("Cancel") );
   mpOk->setText( tr("Ok") );
   setWindowTitle( QApplication::applicationName()+
                   tr("Pattern Sequence Editor") );
}
