/**
 * BarsTabWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "BarsTabWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QCommonStyle>
#include <QDragEnterEvent>
#include <QLabel>
#include <QPushButton>

/* local library headers */
#include <json.h>

/* local headers */
#include "BarWidget.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


BarsTabWidget::BarsTabWidget( SlocumSong *slocumSong, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mCurrentBar( 0 )
, mpChannel0( new BarWidget( mpSlocumSong, mpSlocumSong->getBar(0,0), this ) )
, mpChannel1( new BarWidget( mpSlocumSong, mpSlocumSong->getBar(1,0), this ) )
, mpLeadInLabel( new QLabel( this ) )
, mpLeadOutLabel( new QLabel( this ) )
, mpFirstButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSkipBackward ), "", this ) )
, mpPreviousButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSeekBackward ), "", this ) )
, mpNextButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSeekForward ), "", this ) )
, mpLastButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSkipForward ), "", this ) )
, mpAddBeforeButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_FileDialogNewFolder ), "", this ) )
, mpAddAfterButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_FileDialogNewFolder ), "", this ) )
{
   mpLeadInLabel->setAlignment( Qt::AlignTop | Qt::AlignRight );

   QBoxLayout *layout = new QHBoxLayout( this );
   layout->setContentsMargins( 0, 0, 0, 0 );

   layout->addWidget( mpLeadInLabel );
   layout->addWidget( mpFirstButton );
   layout->addWidget( mpAddBeforeButton );
   layout->addWidget( mpPreviousButton );
   layout->addWidget( mpChannel0 );
   layout->addWidget( mpChannel1 );
   layout->addWidget( mpNextButton );
   layout->addWidget( mpAddAfterButton );
   layout->addWidget( mpLastButton );
   layout->addWidget( mpLeadOutLabel );

   layout->setStretchFactor( mpLeadInLabel, 1 );
   layout->setStretchFactor( mpLeadOutLabel, 1 );

   connect( mpFirstButton, SIGNAL(clicked()),
            this, SLOT(moveFirst()) );
   connect( mpPreviousButton, SIGNAL(clicked()),
            this, SLOT(movePrevious()) );
   connect( mpNextButton, SIGNAL(clicked()),
            this, SLOT(moveNext()) );
   connect( mpLastButton, SIGNAL(clicked()),
            this, SLOT(moveLast()) );
   connect( mpAddBeforeButton, SIGNAL(clicked()),
            this, SLOT(insertBefore()) );
   connect( mpAddAfterButton, SIGNAL(clicked()),
            this, SLOT(insertAfter()) );

   mpFirstButton->setToolTip( tr("First bar") );
   mpPreviousButton->setToolTip( tr("Previous bar") );
   mpNextButton->setToolTip( tr("Next bar") );
   mpLastButton->setToolTip( tr("Last bar") );
   mpAddBeforeButton->setToolTip( tr("Insert new bar before this bar") );
   mpAddAfterButton->setToolTip( tr("Insert new bar after this bar") );

   setTexts();
   setBar( 0 );
}


BarsTabWidget::~BarsTabWidget()
{
}


void BarsTabWidget::setTexts()
{
}


void BarsTabWidget::setFromSong( const SlocumSong *slocumSong )
{
}


void BarsTabWidget::setBar( int bar )
{
   if( bar < 0 )
   {
      bar = 0;
   }
   if( bar >= mpSlocumSong->size() )
   {
      bar = mpSlocumSong->size() - 1;
   }
   if( bar != mCurrentBar )
   {
      mCurrentBar = bar;
      emit barChanged( mCurrentBar );
   }

   bool isFirst  = (bar <= 0);
   bool isLast   = (bar >= mpSlocumSong->size()-1);
   bool maxedOut = (mpSlocumSong->size() == 256); // \todo remove hardwired "256"

   mpLeadInLabel->setText( QString("%1/%2").arg( bar + 1 )
                                           .arg( mpSlocumSong->size() ) );
   mpFirstButton->setDisabled( isFirst );
   mpPreviousButton->setDisabled( isFirst );
   mpNextButton->setDisabled( isLast );
   mpLastButton->setDisabled( isLast );
   mpAddBeforeButton->setDisabled( maxedOut );
   mpAddAfterButton->setDisabled( maxedOut );
   mpChannel0->setFromSong( mpSlocumSong, mpSlocumSong->getBar( 0, bar ) );
   mpChannel1->setFromSong( mpSlocumSong, mpSlocumSong->getBar( 1, bar ) );
}


void BarsTabWidget::moveFirst()
{
   setBar( 0 );
}


void BarsTabWidget::movePrevious()
{
   setBar( mCurrentBar - 1 );
}


void BarsTabWidget::moveNext()
{
   setBar( mCurrentBar + 1 );
}


void BarsTabWidget::moveLast()
{
   setBar( mpSlocumSong->size() - 1 );
}


void BarsTabWidget::insertBefore()
{
   quint8 bar = mCurrentBar & 0xff;
   mpSlocumSong->insertBars( bar );
   emit barAdded();
   setBar( bar );
}


void BarsTabWidget::insertAfter()
{
   quint8 bar = (mCurrentBar + 1) & 0xff;
   mpSlocumSong->insertBars( bar );
   emit barAdded();
   setBar( bar );
}
