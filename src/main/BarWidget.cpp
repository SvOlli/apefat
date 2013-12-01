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
#include <QDragEnterEvent>
#include <QLabel>
#include <QLineEdit>

/* local library headers */
#include <json.h>

/* local headers */
#include "BeatWidget.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


BarWidget::BarWidget( const SlocumSong *slocumSong, SlocumBar *slocumBar, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mpSlocumBar( slocumBar )
, mpSaveBar( 0 )
, mpLayout( new QVBoxLayout( this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpLowVolume( new QCheckBox( this ) )
, mBeats()
{
   setEnabled( slocumBar );
   QBoxLayout *nameLayout = new QHBoxLayout();
   nameLayout->setContentsMargins( 2, 2, 2, 2 );
   nameLayout->setSpacing( 2 );
   nameLayout->addWidget( mpLabelName );
   nameLayout->addWidget( mpValueName, 1 );
   nameLayout->addWidget( mpLowVolume );
   mpLayout->setSpacing( 2 );
   mpLayout->setContentsMargins( 2, 2, 2, 2 );
   mpLayout->addLayout( nameLayout );

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
}


void BarWidget::setLowVolume( bool isLow )
{
   mpSlocumBar->isLow = isLow;
}


void BarWidget::setName( const QString &setname )
{
   mpSlocumBar->name = setname;
}


void BarWidget::setFromSong( const SlocumSong *slocumSong, SlocumBar *slocumBar )
{
   mpSlocumSong  = slocumSong;
   mpSlocumBar   = slocumBar;
   mpValueName->setText( mpSlocumBar->name );
   mpLowVolume->setChecked( mpSlocumBar->isLow );
   for( int i = 0; i < SlocumBar::size(); ++i )
   {
      mBeats.at( i )->setFromSong( mpSlocumSong, &(mpSlocumBar->beat[i]) );
   }
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
