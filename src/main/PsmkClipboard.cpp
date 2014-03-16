/**
 * PsmkClipboard.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkClipboard.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QBoxLayout>
#include <QClipboard>
#include <QLabel>
#include <QMimeData>
#include <QTextEdit>

/* local library headers */

/* local headers */
#include "PsmkBeatWidget.hpp"
#include "PsmkHiHatWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkNoteWidget.hpp"
#include "PsmkPatternWidget.hpp"


PsmkClipboard::PsmkClipboard( QWidget *parent )
: QGroupBox( parent )
, mpMessage1( new QLabel( this ) )
, mpMessage2( new QTextEdit( this ) )
{
   QClipboard *clipboard = QApplication::clipboard();
   QBoxLayout *layout = new QVBoxLayout( this );
   mpMessage2->setReadOnly( true );

   layout->addWidget( mpMessage1 );
   layout->addWidget( mpMessage2 );

   connect( clipboard, SIGNAL(dataChanged()),
            this, SLOT(parseClipboardData()) );

   setTexts();
}


PsmkClipboard::~PsmkClipboard()
{
}


void PsmkClipboard::setTexts()
{
   setTitle( tr("Clipboard:") );
   parseClipboardData();
}


void PsmkClipboard::parseClipboardData()
{
   const QString notext;
   QClipboard *clipboard = QApplication::clipboard();
   const QMimeData *mimeData = clipboard->mimeData();
   QString name( mimeData->data("x-apefat-name") );
   QString named;
   if( !name.isEmpty() )
   {
      named = tr(" named ");
   }
   if( mimeData->hasFormat( PsmkBeatWidget::mimeType() ) )
   {
      mpMessage1->setText( tr("has Beat data%1%2").arg( named, name ) );
      mpMessage2->setText( mimeData->data( PsmkBeatWidget::mimeType() ) );
   }
   else if( mimeData->hasFormat( PsmkHiHatWidget::mimeType() ) )
   {
      mpMessage1->setText( tr("has HiHat data%1%2").arg( named, name ) );
      mpMessage2->setText( mimeData->data( PsmkHiHatWidget::mimeType() ) );
   }
   else if( mimeData->hasFormat( PsmkInstrumentsWidget::mimeType() ) )
   {
      mpMessage1->setText( tr("has Instruments data%1%2").arg( named, name ) );
      mpMessage2->setText( mimeData->data( PsmkInstrumentsWidget::mimeType() ) );
   }
   else if( mimeData->hasFormat( PsmkNoteWidget::mimeType() ) )
   {
      mpMessage1->setText( tr("has Note data") );
      mpMessage2->setText( mimeData->data( PsmkNoteWidget::mimeType() ) );
   }
   else if( mimeData->hasFormat( PsmkPatternWidget::mimeType() ) )
   {
      mpMessage1->setText( tr("has Pattern data%1%2").arg( named, name ) );
      mpMessage2->setText( mimeData->data( PsmkPatternWidget::mimeType() ) );
   }
   else if( mimeData->hasText() )
   {
      mpMessage1->setText( tr("has text") );
      mpMessage2->setText( mimeData->text() );
   }
   else
   {
      mpMessage1->setText( tr("has no/unknown data") );
      mpMessage2->setText( notext );
   }
}
