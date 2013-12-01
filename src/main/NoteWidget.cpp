/**
 * NoteWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "NoteWidget.hpp"

/* system headers */
#include <cstring>

/* Qt headers */
#include <QBoxLayout>
#include <QCheckBox>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPicture>

/* local library headers */
#include "json.h"

/* local headers */
#include "PitchComboBox.hpp"
#include "SlocumSong.hpp"
#include "SoundComboBox.hpp"


#include <QtDebug>


const char NoteWidget::cMimeType[] = "x-vcs-tracker-note";
bool       NoteWidget::cDragImage  = false;


NoteWidget::NoteWidget( const SlocumSong *slocumSong, SlocumNote *slocumNote, int index, QWidget *parent )
: QWidget( parent )
, mpSlocumSong( slocumSong )
, mpSlocumNote( slocumNote )
, mIndex( index )
, mpLayout( new QHBoxLayout( this ) )
, mpSound( new SoundComboBox( mpSlocumSong, this ) )
, mpPitch( new PitchComboBox( this ) )
, mpAccent( new QCheckBox( this ) )
{
   mpLayout->setContentsMargins( 0, 0, 0, 0 );
   mpLayout->addWidget( mpSound );
   mpLayout->addWidget( mpPitch );
   mpLayout->addWidget( mpAccent );
   mpLayout->addStretch();
   //mpLayout->addStretch();
   connect( mpSound, SIGNAL(valueChanged(quint8)),
            this, SLOT(setSoundValue(quint8)) );
   connect( mpPitch, SIGNAL(valueChanged(quint8)),
            this, SLOT(setPitchValue(quint8)) );
   connect( mpAccent, SIGNAL(toggled(bool)),
            this, SLOT(setAccent(bool)) );
   // workaround for setting notelist in pitch correctly
   mpSound->setValue( 255 );
}


NoteWidget::~NoteWidget()
{
}


const char *NoteWidget::mimeType()
{
   return &cMimeType[0];
}


int NoteWidget::index() const
{
   return mIndex;
}


void NoteWidget::setSoundValue( quint8 sound )
{
   mpSlocumNote->sound = sound;
   mpSound->setValue( sound );
   if( sound < SlocumSound::size() )
   {
      mpPitch->setSound( mpSlocumSong->sound()->type[sound] );
   }
   else
   {
      mpPitch->setSound( sound );
   }
   mpAccent->setDisabled( sound == 255 );
}


void NoteWidget::setPitchValue( quint8 pitch )
{
   if( mpSound->value() == 255 )
   {
      pitch = 255;
   }
   mpSlocumNote->pitch = pitch;
   mpPitch->setValue( pitch );
}


void NoteWidget::setAccent( bool setaccent )
{
   if( mpSound->value() == 255 )
   {
      setaccent = false;
   }
   mpSlocumNote->accent = setaccent;
   mpAccent->setChecked( setaccent );
}


void NoteWidget::setValues( const SlocumNote &note )
{
   quint8 sound = note.sound;
   mpSound->setFromSong( mpSlocumSong );
   mpSound->setValue( sound );
   mpPitch->setValue( note.pitch );
   mpAccent->setChecked( note.accent );
}


void NoteWidget::setCreateDragImage( bool on )
{
   cDragImage = on;
}


void NoteWidget::setFromSong( const SlocumSong *slocumSong, SlocumNote *slocumNote )
{
   mpSlocumSong = slocumSong;
   mpSlocumNote = slocumNote;

   setValues( *slocumNote );
}


void NoteWidget::mousePressEvent( QMouseEvent *event )
{
   QMimeData *mimeData = new QMimeData();
   mimeData->setData( NoteWidget::mimeType(), QtJson::serialize( mpSlocumNote->toVariantMap() ) );
   mimeData->setData( "index", QByteArray::number(mIndex) );
   // for debugging only:
   //mimeData->setText( QString::fromUtf8(mimeData->data( NoteWidget::mimeType() )) );

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
