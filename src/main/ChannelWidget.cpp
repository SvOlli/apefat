/**
 * ChannelWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ChannelWidget.hpp"

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
#include "BarWidget.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


ChannelWidget::ChannelWidget( SlocumSong *slocumSong, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
{
   setTexts();
   setAcceptDrops( true );
}


ChannelWidget::~ChannelWidget()
{
}


void ChannelWidget::setTexts()
{
}


void ChannelWidget::setFromSong( const SlocumSong *slocumSong, SlocumBar *slocumBar )
{
}


void ChannelWidget::dragEnterEvent( QDragEnterEvent *event )
{
   dragMoveEvent( event );
}


void ChannelWidget::dragMoveEvent( QDragMoveEvent *event )
{
#if 0
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasFormat( BarWidget::mimeType() ) )
   {
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
#endif
}


void ChannelWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
#if 0
   if( mpSaveBar )
   {
      setValues( *mpSaveBar );
      delete mpSaveBar;
      mpSaveBar = 0;
   }
   event->accept();
#endif
}


void ChannelWidget::dropEvent( QDropEvent *event )
{
#if 0
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
#endif
}


void ChannelWidget::moveBar( int from, int to )
{
}


BarWidget *ChannelWidget::barWidgetAt( const QPoint &pos )
{
   BarWidget *barWidget = 0;
   for( QObject *o = childAt( pos ); o && (o != this); o = o->parent() )
   {
      barWidget = qobject_cast<BarWidget*>(o);
   }
   return barWidget;
}
