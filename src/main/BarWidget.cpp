/**
 * BarWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "BarWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QCheckBox>
#include <QCommonStyle>
#include <QDragEnterEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

/* local library headers */
#include <json.h>

/* local headers */
#include "BeatWidget.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


BarWidget::BarWidget( SlocumSong *slocumSong, int voice, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mpSlocumBar( mpSlocumSong->voice( voice )->bar( 0 ) )
, mpSaveBar( 0 )
, mpLayout( new QVBoxLayout( this ) )
, mpPositionText( new QLabel( this ) )
, mpFirstButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSkipBackward ), "", this ) )
, mpPreviousButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSeekBackward ), "", this ) )
, mpNextButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSeekForward ), "", this ) )
, mpLastButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_MediaSkipForward ), "", this ) )
, mpAddBeforeButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_FileDialogNewFolder ), "", this ) )
, mpAddAfterButton( new QPushButton( QCommonStyle().standardIcon( QStyle::SP_FileDialogNewFolder ), "", this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpLowVolume( new QCheckBox( this ) )
, mVoice( voice )
, mBar( 0 )
, mBeats()
{
   QBoxLayout *posLayout  = new QHBoxLayout();
   QBoxLayout *nameLayout = new QHBoxLayout();
   mpLayout->addLayout( posLayout );
   mpLayout->addLayout( nameLayout );

   posLayout->addWidget( mpFirstButton );
   posLayout->addWidget( mpAddBeforeButton );
   posLayout->addWidget( mpPreviousButton );
   posLayout->addWidget( mpPositionText );
   posLayout->addWidget( mpNextButton );
   posLayout->addWidget( mpAddAfterButton );
   posLayout->addWidget( mpLastButton );
   posLayout->setStretch( 3, 1 );

   nameLayout->addWidget( mpLabelName );
   nameLayout->addWidget( mpValueName );
   nameLayout->addWidget( mpLowVolume );
   nameLayout->setStretch( 1, 1 );

   for( int i = 0; i < SlocumBar::size(); ++i )
   {
      BeatWidget *beat = new BeatWidget( mpSlocumSong, &(mpSlocumBar->beat[i]), i, this );
      mBeats.append( beat );
      mpLayout->addWidget( beat );
   }
   mpLowVolume->setChecked( mpSlocumBar->isLow );
   connect( mpLowVolume, SIGNAL(clicked(bool)),
            this, SLOT(setLowVolume(bool)) );
   connect( mpValueName, SIGNAL(textChanged(QString)),
            this, SLOT(setName(QString)) );

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

   setTexts();
   setAcceptDrops( true );
}


BarWidget::~BarWidget()
{
}


void BarWidget::setTexts()
{
   mpLabelName->setText( tr("Name:") );
   mpLowVolume->setText( tr("Low Volume") );
   mpPositionText->setText( tr("Voice %1 Bar %2/%3")
                            .arg( mVoice )
                            .arg( mBar + 1, 3, 10, QChar('0') )
                            .arg( mpSlocumSong->size( mVoice ), 3, 10, QChar('0') ) );
}


void BarWidget::setLowVolume( bool isLow )
{
   mpSlocumBar->isLow = isLow;
}


void BarWidget::setName( const QString &setname )
{
   mpSlocumBar->name = setname;
}


void BarWidget::setFromSong( SlocumSong *slocumSong, quint8 bar )
{
   mpSlocumSong  = slocumSong;
   mpSlocumBar   = mpSlocumSong->voice( mVoice )->bar( bar );
   mpValueName->setText( mpSlocumBar->name );
   mpLowVolume->setChecked( mpSlocumBar->isLow );
   for( int i = 0; i < SlocumBar::size(); ++i )
   {
      mBeats.at( i )->setFromSong( mpSlocumSong, &(mpSlocumBar->beat[i]) );
   }

   bool isFirst  = (bar <= 0);
   bool isLast   = (bar >= mpSlocumSong->size( mVoice )-1);
   bool maxedOut = (mpSlocumSong->size( mVoice ) == 256); // \todo remove hardwired "256"

   mpFirstButton->setDisabled( isFirst );
   mpPreviousButton->setDisabled( isFirst );
   mpNextButton->setDisabled( isLast );
   mpLastButton->setDisabled( isLast );
   mpAddBeforeButton->setDisabled( maxedOut );
   mpAddAfterButton->setDisabled( maxedOut );

   setTexts();
}


void BarWidget::setValues( const SlocumBar &bar )
{
   mpValueName->setText( bar.name );
   mpLowVolume->setChecked( bar.isLow );
   Q_ASSERT( mBeats.size() == SlocumBar::size() );
   for( int i = 0; i < SlocumBar::size(); ++i )
   {
      mBeats.at( i )->setValues( bar.beat[i] );
   }
}


void BarWidget::setBar( int bar )
{
   if( bar >= mpSlocumSong->size( mVoice ) )
   {
      bar = mpSlocumSong->size( mVoice ) - 1;
   }
   if( bar < 0 )
   {
      bar = 0;
   }
   if( bar != mBar )
   {
      mBar = bar;
      emit barChanged( mBar );
   }

   setFromSong( mpSlocumSong, mBar );
}


void BarWidget::dragEnterEvent( QDragEnterEvent *event )
{
   dragMoveEvent( event );
}


void BarWidget::dragMoveEvent( QDragMoveEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( !mpSaveBar )
   {
      mpSaveBar = new SlocumBar( *mpSlocumBar );
   }
   if( mimeData->hasFormat( BeatWidget::mimeType() ) )
   {
      if( mpSaveBar )
      {
         setValues( *mpSaveBar );
         BeatWidget *beatWidget = beatWidgetAt( event->pos() );
         if( beatWidget )
         {
            if( children().contains( event->source() ) )
            {
               bool ok;
               int fromIndex = mimeData->data( "index" ).toInt( &ok );
               Q_ASSERT( ok );
               if( (fromIndex < 0) || (fromIndex >= mBeats.count()) )
               {
                  return;
               }
               moveBeat( fromIndex, beatWidget->index() );
               event->setDropAction( Qt::MoveAction );
            }
            else
            {
               QString jsonData( QString::fromUtf8( mimeData->data( BeatWidget::mimeType() ) ) );
               beatWidget->setValues( SlocumBeat( QtJson::parse( jsonData ).toMap() ) );
               event->setDropAction( Qt::CopyAction );
            }
            event->accept();
         }
         else
         {
            if( mpSaveBar )
            {
               setValues( *mpSaveBar );
               delete mpSaveBar;
               mpSaveBar = 0;
            }
         }
      }
   }
}


void BarWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
   if( mpSaveBar )
   {
      setValues( *mpSaveBar );
      delete mpSaveBar;
      mpSaveBar = 0;
   }
   event->accept();
}


void BarWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasFormat( BeatWidget::mimeType() ) )
   {
      if( mpSaveBar )
      {
         delete mpSaveBar;
         mpSaveBar = 0;
      }
      if( beatWidgetAt( event->pos() ) )
      {
         event->acceptProposedAction();
      }
   }
}


void BarWidget::moveBeat( int from, int to )
{
   SlocumBeat saveBeat = mpSlocumBar->beat[from];

   int direction = ( from < to ) ? 1 : -1;
   int last = from;
   for( int i = from + direction; i != to + direction; i += direction )
   {
      mBeats.at( last )->setValues( mpSlocumBar->beat[i] );
      last = i;
   }

   mBeats.at( to )->setValues( saveBeat );
}


BeatWidget *BarWidget::beatWidgetAt( const QPoint &pos )
{
   BeatWidget *beatWidget = 0;
   for( QObject *o = childAt( pos ); o && (o != this); o = o->parent() )
   {
      beatWidget = qobject_cast<BeatWidget*>(o);
   }
   return beatWidget;
}


void BarWidget::moveFirst()
{
   setBar( 0 );
}


void BarWidget::movePrevious()
{
   setBar( mBar - 1 );
}


void BarWidget::moveNext()
{
   setBar( mBar + 1 );
}


void BarWidget::moveLast()
{
   setBar( mpSlocumSong->size( mVoice ) - 1 );
}


void BarWidget::insertBefore()
{
   quint8 bar = mBar & 0xff;
   mpSlocumSong->voice( mVoice )->insert( bar, SlocumBar() );
   setBar( bar );
}


void BarWidget::insertAfter()
{
   quint8 bar = (mBar + 1) & 0xff;
   mpSlocumSong->voice( mVoice )->insert( bar, SlocumBar() );
   setBar( bar );
}
