/**
 * PsmkBeatWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkBeatWidget.hpp"

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
#include "PsmkConfig.hpp"
#include "PsmkNoteWidget.hpp"
#include "PsmkContextMenu.hpp"
#include "QxtLabel.hpp"


#include <QtDebug>


const char PsmkBeatWidget::cMimeType[] = "x-apefat-beat";
bool       PsmkBeatWidget::cDragImage  = false;


PsmkBeatWidget::PsmkBeatWidget( int index, QWidget *parent )
: QWidget( parent )
, mIndex( index )
, mpTypeName( new QxtLabel( this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpNotes()
, mSaveBeat()
{
   setup();
}


PsmkBeatWidget::PsmkBeatWidget( int index, quint8 *instruments, QWidget *parent )
: QWidget( parent )
, mIndex( index )
, mpTypeName( new QxtLabel( this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpNotes()
, mSaveBeat()
{
   setup( instruments );
}


void PsmkBeatWidget::setup( quint8 *instruments )
{
   QBoxLayout *mainLayout  = new QHBoxLayout( this );
   QBoxLayout *notesLayout = new QVBoxLayout();
   QBoxLayout *nameLayout  = new QHBoxLayout();

   nameLayout->setContentsMargins( 2, 2, 2, 2 );
   nameLayout->setSpacing( 2 );
   nameLayout->addWidget( mpLabelName );
   nameLayout->addWidget( mpValueName, 1 );

   notesLayout->setSpacing( 1 );
   notesLayout->addLayout( nameLayout );
   notesLayout->setContentsMargins( 2, 2, 2, 0 );

   mainLayout->setSpacing( 0 );
   mainLayout->addLayout( notesLayout );
   mpTypeName->setRotation( QxtLabel::Clockwise );
   mpTypeName->setFrameStyle( QFrame::Box );
   mpTypeName->setLineWidth( 1 );
   mainLayout->addWidget( mpTypeName );
   mainLayout->setContentsMargins( 0, 0, 0, 0 );

   for( int i = 0; i < PsmkConfig::NotesInBeat; ++i )
   {
      mpNotes[i] = new PsmkNoteWidget( i, instruments, this );
      notesLayout->addWidget( mpNotes[i] );
      connect( mpNotes[i], SIGNAL(changed()),
               this, SIGNAL(changed()) );
   }
   setTexts();
   setAcceptDrops( true );
}


PsmkBeatWidget::~PsmkBeatWidget()
{
}


const char *PsmkBeatWidget::mimeType()
{
   return &cMimeType[0];
}


void PsmkBeatWidget::setCreateDragImage( bool on )
{
   cDragImage = on;
}


QString PsmkBeatWidget::name()
{
   return mpValueName->text();
}


void PsmkBeatWidget::setInstrument( int index, quint8 tone )
{
   for( int i = 0; i < PsmkConfig::NotesInBeat; ++i )
   {
      mpNotes[i]->setInstrument( index, tone );
   }
}


void PsmkBeatWidget::setInstruments( quint8 tones[] )
{
   for( int i = 0; i < PsmkConfig::NotesInBeat; ++i )
   {
      mpNotes[i]->setInstruments( tones );
   }
}


int PsmkBeatWidget::index() const
{
   return mIndex;
}


QVariantMap PsmkBeatWidget::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList notes;

   for( int i = 0; i < PsmkConfig::NotesInBeat; ++i )
   {
      notes.append( mpNotes[i]->toVariantMap() );
   }
   variantMap.insert( "name", mpValueName->text() );
   variantMap.insert( "notes", notes );

   return variantMap;
}


bool PsmkBeatWidget::fromVariantMap( const QVariantMap &variantMap )
{
   Q_ASSERT( variantMap.contains( "name" ) );
   mpValueName->setText( variantMap.value( "name" ).toString() );

   Q_ASSERT( variantMap.contains( "notes" ) );
   QVariantList notes( variantMap.value("notes").toList() );
   Q_ASSERT( notes.size() == PsmkConfig::NotesInBeat );

   for( int i = 0; i < notes.size(); ++i )
   {
      mpNotes[i]->fromVariantMap( notes.at(i).toMap() );
   }
   return true;
}


QByteArray PsmkBeatWidget::toBinary() const
{
   QByteArray retval;
   char accent = 0;
   QByteArray note;
   for( int i = 0; i < PsmkConfig::NotesInBeat; ++i )
   {
      note = mpNotes[i]->toBinary();
      retval.append( note.at(0) );
      accent |= note.at(1);
   }
   retval.append( accent );
   return retval;
}


void PsmkBeatWidget::setTexts()
{
   mpTypeName->setText( tr("Beat") );
   mpLabelName->setText( tr("Name:") );
}


void PsmkBeatWidget::dragEnterEvent( QDragEnterEvent *event )
{
   dragMoveEvent( event );
}


void PsmkBeatWidget::dragMoveEvent( QDragMoveEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mSaveBeat.isEmpty() )
   {
      mSaveBeat = toVariantMap();
   }
   if( mimeData->hasFormat( PsmkNoteWidget::mimeType() ) )
   {
      fromVariantMap( mSaveBeat );
      PsmkNoteWidget *noteWidget = noteWidgetAt( event->pos() );
      if( noteWidget )
      {
         if( children().contains( event->source() ) )
         {
            bool ok;
            int fromIndex = mimeData->data( "index" ).toInt( &ok );
            Q_ASSERT( ok );
            if( (fromIndex < 0) || (fromIndex >= PsmkConfig::NotesInBeat) )
            {
               return;
            }
            moveNote( fromIndex, noteWidget->index() );
            event->setDropAction( Qt::MoveAction );
         }
         else
         {
            QString jsonData( QString::fromUtf8( mimeData->data( PsmkNoteWidget::mimeType() ) ) );
            noteWidget->fromVariantMap( QtJson::parse( jsonData ).toMap() );
            event->setDropAction( Qt::CopyAction );
         }
         event->accept();
      }
      else
      {
         if( !mSaveBeat.isEmpty() )
         {
            mSaveBeat.clear();
         }
      }
   }
}


void PsmkBeatWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
   if( !mSaveBeat.isEmpty() )
   {
      fromVariantMap( mSaveBeat );
      mSaveBeat.clear();
   }
   event->accept();
}


void PsmkBeatWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasFormat( PsmkNoteWidget::mimeType() ) )
   {
      if( !mSaveBeat.isEmpty() )
      {
         mSaveBeat.clear();
      }
      if( noteWidgetAt( event->pos() ) )
      {
         event->acceptProposedAction();
      }
   }
}


#if 0
void PsmkBeatWidget::paintEvent( QPaintEvent *event )
{
   QWidget::paintEvent( event );

   QPainter painter;
   painter.begin( this );
   painter.setPen( QColor( 0, 0, 0 ) );
   painter.drawRect( 0, 0, width()-1, height()-1 );
   painter.end();
}
#endif


void PsmkBeatWidget::moveNote( int from, int to )
{
   QVariantMap saveNote = mpNotes[from]->toVariantMap();

   int direction = ( from < to ) ? 1 : -1;
   int last = from;
   for( int i = from + direction; i != to + direction; i += direction )
   {
      mpNotes[last]->fromVariantMap( mpNotes[i]->toVariantMap() );
      last = i;
   }

   mpNotes[to]->fromVariantMap( saveNote );
}


PsmkNoteWidget *PsmkBeatWidget::noteWidgetAt( const QPoint &pos )
{
   PsmkNoteWidget *noteWidget = 0;
   for( QObject *o = childAt( pos ); o && (o != this); o = o->parent() )
   {
      noteWidget = qobject_cast<PsmkNoteWidget*>(o);
   }
   return noteWidget;
}


void PsmkBeatWidget::mousePressEvent( QMouseEvent *event )
{
   QMimeData *mimeData = new QMimeData();
   mimeData->setData( PsmkBeatWidget::mimeType(), QtJson::serialize( toVariantMap() ) );
   mimeData->setData( "index", QByteArray::number(mIndex) );
   // for debugging only:
   //mimeData->setText( QString::fromUtf8(mimeData->data( PsmkBeatWidget::mimeType() )) );

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


void PsmkBeatWidget::contextMenuEvent( QContextMenuEvent *event )
{
   PsmkContextMenu menu( tr("Beat"), cMimeType, toVariantMap(), this );

   if( menu.runSelect( event->globalPos() - QPoint( 5, 5 ) ) == PsmkContextMenu::Paste )
   {
      fromVariantMap( menu.mimeData().toMap() );
   }
}
