/**
 * BeatWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "BeatWidget.hpp"

/* system headers */

/* Qt headers */
#include <QBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMimeData>
#include <QPainter>
#include <QSignalMapper>
#include <QTimer>

/* local library headers */
#include <json.h>

/* local headers */
#include "NoteWidget.hpp"
#include "SlocumSong.hpp"


#include <QtDebug>


const char BeatWidget::cMimeType[] = "x-apefat-beat";
bool       BeatWidget::cDragImage  = false;


BeatWidget::BeatWidget( const SlocumSong *slocumSong, SlocumBeat *slocumBeat, int index, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mpSlocumBeat( slocumBeat )
, mpSaveBeat( 0 )
, mIndex( index )
, mpLayout( new QVBoxLayout( this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mNotes()
{
   QBoxLayout *nameLayout = new QHBoxLayout();
   nameLayout->setContentsMargins( 2, 2, 2, 2 );
   nameLayout->setSpacing( 2 );
   nameLayout->addWidget( mpLabelName );
   nameLayout->addWidget( mpValueName, 1 );
   mpLayout->setSpacing( 0 );
   mpLayout->addLayout( nameLayout );
   mpLayout->setContentsMargins( 2, 2, 2, 2 );

   for( int i = 0; i < SlocumBeat::size(); ++i )
   {
      NoteWidget *note = new NoteWidget( mpSlocumSong, &(mpSlocumBeat->note[i]), i, this );
      mNotes.append( note );
      mpLayout->addWidget( note );
      note->setValues( mpSlocumBeat->note[i] );
   }
   connect( mpValueName, SIGNAL(textChanged(QString)),
            this, SLOT(setName(QString)) );
   setTexts();
   setAcceptDrops( true );
}


BeatWidget::~BeatWidget()
{
}


const char *BeatWidget::mimeType()
{
   return &cMimeType[0];
}


void BeatWidget::setCreateDragImage( bool on )
{
   cDragImage = on;
}


int BeatWidget::index() const
{
   return mIndex;
}


void BeatWidget::setValues( const SlocumBeat &beat )
{
   mpValueName->setText( beat.name );
   Q_ASSERT( mNotes.size() == SlocumBeat::size() );
   for( int i = 0; i < SlocumBeat::size(); ++i )
   {
      mNotes.at( i )->setValues( beat.note[i] );
   }
}


void BeatWidget::setTexts()
{
   mpLabelName->setText( tr("Name:") );
}


void BeatWidget::setName( const QString &setname )
{
   mpSlocumBeat->name = setname;
}


void BeatWidget::setFromSong( const SlocumSong *slocumSong, SlocumBeat *slocumBeat )
{
   mpSlocumSong  = slocumSong;
   mpSlocumBeat  = slocumBeat;
   mpValueName->setText( mpSlocumBeat->name );
   for( int i = 0; i < SlocumBeat::size(); ++i )
   {
      mNotes.at( i )->setFromSong( mpSlocumSong, &(mpSlocumBeat->note[i]) );
   }
}


void BeatWidget::dragEnterEvent( QDragEnterEvent *event )
{
   dragMoveEvent( event );
}


void BeatWidget::dragMoveEvent( QDragMoveEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( !mpSaveBeat )
   {
      mpSaveBeat = new SlocumBeat( *mpSlocumBeat );
   }
   if( mimeData->hasFormat( NoteWidget::mimeType() ) )
   {
      setValues( *mpSaveBeat );
      NoteWidget *noteWidget = noteWidgetAt( event->pos() );
      if( noteWidget )
      {
         if( children().contains( event->source() ) )
         {
            bool ok;
            int fromIndex = mimeData->data( "index" ).toInt( &ok );
            Q_ASSERT( ok );
            if( (fromIndex < 0) || (fromIndex >= mNotes.count()) )
            {
               return;
            }
            moveNote( fromIndex, noteWidget->index() );
            event->setDropAction( Qt::MoveAction );
         }
         else
         {
            QString jsonData( QString::fromUtf8( mimeData->data( NoteWidget::mimeType() ) ) );
            noteWidget->setValues( SlocumNote( QtJson::parse( jsonData ).toMap() ) );
            mpSlocumBeat->note[noteWidget->index()] = SlocumNote( QtJson::parse( jsonData ).toMap() );
            event->setDropAction( Qt::CopyAction );
         }
         event->accept();
      }
      else
      {
         if( mpSaveBeat )
         {
            setValues( *mpSaveBeat );
            delete mpSaveBeat;
            mpSaveBeat = 0;
         }
      }
   }
}


void BeatWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
   if( mpSaveBeat )
   {
      setValues( *mpSaveBeat );
      delete mpSaveBeat;
      mpSaveBeat = 0;
   }
   event->accept();
}


void BeatWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasFormat( NoteWidget::mimeType() ) )
   {
      if( mpSaveBeat )
      {
         delete mpSaveBeat;
         mpSaveBeat = 0;
      }
      if( noteWidgetAt( event->pos() ) )
      {
         event->acceptProposedAction();
      }
   }
}


void BeatWidget::paintEvent( QPaintEvent *event )
{
   QWidget::paintEvent( event );

   QPainter painter;
   painter.begin( this );
   painter.setPen( QColor( 0, 0, 0 ) );
   painter.drawRect( 0, 0, width()-1, height()-1 );
   painter.end();
}


void BeatWidget::moveNote( int from, int to )
{
   SlocumNote saveNote = mpSlocumBeat->note[from];

   int direction = ( from < to ) ? 1 : -1;
   int last = from;
   for( int i = from + direction; i != to + direction; i += direction )
   {
      mNotes.at( last )->setValues( mpSlocumBeat->note[i] );
      last = i;
   }

   mNotes.at( to )->setValues( saveNote );
}


NoteWidget *BeatWidget::noteWidgetAt( const QPoint &pos )
{
   NoteWidget *noteWidget = 0;
   for( QObject *o = childAt( pos ); o && (o != this); o = o->parent() )
   {
      noteWidget = qobject_cast<NoteWidget*>(o);
   }
   return noteWidget;
}


void BeatWidget::mousePressEvent( QMouseEvent *event )
{
   QMimeData *mimeData = new QMimeData();
   mimeData->setData( BeatWidget::mimeType(), QtJson::serialize( mpSlocumBeat->toVariantMap() ) );
   mimeData->setData( "index", QByteArray::number(mIndex) );
   // for debugging only:
   //mimeData->setText( QString::fromUtf8(mimeData->data( BeatWidget::mimeType() )) );

   QWidget *w = focusWidget();
   if( w )
   {
      w->clearFocus();
   }
   QDrag *drag = new QDrag( this );
   drag->setMimeData( mimeData );
   if( cDragImage )
   {
      QPixmap pixmap( width()+2, height()+2 );
      pixmap.fill( QColor(0,0,0) );
      QPainter painter;
      painter.begin( &pixmap );
      painter.drawImage( 1, 1, QPixmap::grabWidget( this ).toImage() );
      painter.end();
      drag->setPixmap( pixmap );
      drag->setHotSpot( QPoint( event->x()+1, event->y()+1 ) );
   }
   // Qt::DropAction action =
   drag->exec( Qt::CopyAction | Qt::MoveAction, Qt::MoveAction );
   event->accept();
}
