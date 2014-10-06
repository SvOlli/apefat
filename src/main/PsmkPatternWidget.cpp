/**
 * PsmkPatternWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkPatternWidget.hpp"

/* system headers */

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QBoxLayout>
#include <QCheckBox>
#include <QCommonStyle>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#else
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#endif
#include <QDragEnterEvent>
#include <QMimeData>

/* local library headers */
#include <json.h>

/* local headers */
#include "PsmkBeatWidget.hpp"
#include "PsmkContextMenu.hpp"
#include "PsmkPacker.hpp"
#include "QxtLabel.hpp"


#include <QtDebug>


const char PsmkPatternWidget::cMimeType[] = "x-apefat-pattern";


PsmkPatternWidget::PsmkPatternWidget( QWidget *parent )
: QWidget( parent )
, mpTypeName( new QxtLabel( this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpLowVolume( new QCheckBox( this ) )
, mpBeats()
, mSavePattern()
{
   setup();
}


PsmkPatternWidget::PsmkPatternWidget( quint8 *instruments, QWidget *parent )
: QWidget( parent )
, mpTypeName( new QxtLabel( this ) )
, mpLabelName( new QLabel( this ) )
, mpValueName( new QLineEdit( this ) )
, mpLowVolume( new QCheckBox( this ) )
, mpBeats()
, mSavePattern()
{
   setup( instruments );
}


void PsmkPatternWidget::setup( quint8 *instruments )
{
   QBoxLayout *mainLayout  = new QHBoxLayout( this );
   QBoxLayout *beatsLayout = new QVBoxLayout();
   QBoxLayout *nameLayout  = new QHBoxLayout();

   mpTypeName->setRotation( QxtLabel::Clockwise );
   mpTypeName->setFrameStyle( QFrame::Box );
   mpTypeName->setLineWidth( 1 );

   mpLowVolume->setContentsMargins( 0, 2, 0, 2 );

   nameLayout->addWidget( mpLabelName );
   nameLayout->addWidget( mpValueName );
   nameLayout->addWidget( mpLowVolume );
   nameLayout->setStretch( 1, 1 );
   nameLayout->setContentsMargins( 0, 0, 0, 0 );

   beatsLayout->setSpacing( 1 );
   beatsLayout->addLayout( nameLayout );
   beatsLayout->setContentsMargins( 2, 2, 2, 0 );

   mainLayout->setSpacing( 0 );
   mainLayout->addLayout( beatsLayout );
   mainLayout->addWidget( mpTypeName );
   mainLayout->setContentsMargins( 0, 0, 0, 0 );

   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      mpBeats[i] = new PsmkBeatWidget( i, instruments, this );
      beatsLayout->addWidget( mpBeats[i] );
      connect( mpBeats[i], SIGNAL(changed()),
               this, SIGNAL(changed()) );
   }
   connect( mpLowVolume, SIGNAL(toggled(bool)),
            this, SIGNAL(changed()) );

   setTexts();
   setAcceptDrops( true );
}


PsmkPatternWidget::~PsmkPatternWidget()
{
}


const char *PsmkPatternWidget::mimeType()
{
   return &cMimeType[0];
}


QVariantMap PsmkPatternWidget::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList beats;

   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      beats.append( mpBeats[i]->toVariantMap() );
   }
   variantMap.insert( "name",  mpValueName->text() );
   variantMap.insert( "islow", mpLowVolume->isChecked() );
   variantMap.insert( "beats", beats );

   return variantMap;
}


bool PsmkPatternWidget::fromVariantMap( const QVariantMap &variantMap )
{
   if( !variantMap.contains( "name" ) ||
       !variantMap.contains( "islow" ) ||
       !variantMap.contains( "beats" ) )
   {
      return false;
   }
   QVariantList beats( variantMap.value("beats").toList() );
   if( beats.size() != PsmkConfig::BeatsInPattern )
   {
      return false;
   }
   mpValueName->setText( variantMap.value( "name" ).toString() );
   mpLowVolume->setChecked( variantMap.value( "islow" ).toBool() );
   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      mpBeats[i]->fromVariantMap( beats.at(i).toMap() );
   }
   return true;
}


QByteArray PsmkPatternWidget::toBinary() const
{
   QByteArray retval;
   retval.append( mpLowVolume->isChecked() ? 1 : 0 );
   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      retval.append( mpBeats[i]->toBinary() );
   }
   return retval;
}


QStringList PsmkPatternWidget::names( const PsmkPacker *psmkPacker ) const
{
   QStringList retval( mpValueName->text() );
   QString beatText;
   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      beatText = mpBeats[i]->name();
      if( beatText.isEmpty() )
      {
         beatText = tr("(unnamed)");
      }

      if( psmkPacker )
      {
         beatText.append( QString(" (%1)").arg( psmkPacker->beatCount( mpBeats[i] ) ) );
      }
      retval.append( beatText );
   }
   return retval;
}


PsmkBeatWidget *PsmkPatternWidget::beat( int i ) const
{
   if( (i < 0) || (i > PsmkConfig::BeatsInHiHat) )
   {
      return (PsmkBeatWidget*)0;
   }
   return mpBeats[i];
}


PsmkPatternWidget *PsmkPatternWidget::getClone( QWidget *parent ) const
{
   PsmkPatternWidget *cloned = new PsmkPatternWidget( parent );
   cloned->fromVariantMap( toVariantMap() );
   return cloned;
}


void PsmkPatternWidget::setInstrument( int index, quint8 tone )
{
   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      mpBeats[i]->setInstrument( index, tone );
   }
}


void PsmkPatternWidget::setInstruments( quint8 tones[] )
{
   for( int i = 0; i < PsmkConfig::BeatsInPattern; ++i )
   {
      mpBeats[i]->setInstruments( tones );
   }
}


void PsmkPatternWidget::setTexts()
{
   mpTypeName->setText( tr("Pattern") );
   mpLabelName->setText( tr("Name:") );
   mpLowVolume->setText( tr("Low Volume") );
}


void PsmkPatternWidget::dragEnterEvent( QDragEnterEvent *event )
{
   dragMoveEvent( event );
}


void PsmkPatternWidget::dragMoveEvent( QDragMoveEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mSavePattern.isEmpty() )
   {
      mSavePattern = toVariantMap();
   }
   if( mimeData->hasFormat( PsmkBeatWidget::mimeType() ) )
   {
      if( !mSavePattern.isEmpty() )
      {
         PsmkBeatWidget *beatWidget = beatWidgetAt( event->pos() );
         if( beatWidget )
         {
            if( children().contains( event->source() ) )
            {
               bool ok;
               int fromIndex = mimeData->data( "index" ).toInt( &ok );
               Q_ASSERT( ok );
               if( (fromIndex < 0) || (fromIndex >= PsmkConfig::BeatsInPattern) )
               {
                  return;
               }
               moveBeat( fromIndex, beatWidget->index() );
               event->setDropAction( Qt::MoveAction );
            }
            else
            {
               QString jsonData( QString::fromUtf8( mimeData->data( PsmkBeatWidget::mimeType() ) ) );
               beatWidget->fromVariantMap( QtJson::parse( jsonData ).toMap() );
               event->setDropAction( Qt::CopyAction );
            }
            event->accept();
         }
         else
         {
            if( !mSavePattern.isEmpty() )
            {
               mSavePattern.clear();
            }
         }
      }
   }
}


void PsmkPatternWidget::dragLeaveEvent( QDragLeaveEvent *event )
{
   if( !mSavePattern.empty() )
   {
      fromVariantMap( mSavePattern );
      mSavePattern.clear();
   }
   event->accept();
}


void PsmkPatternWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();
   if( mimeData->hasFormat( PsmkBeatWidget::mimeType() ) )
   {
      if( !mSavePattern.isEmpty() )
      {
         mSavePattern.clear();
      }
      if( beatWidgetAt( event->pos() ) )
      {
         event->acceptProposedAction();
      }
   }
}


void PsmkPatternWidget::moveBeat( int from, int to )
{
   QVariantMap saveNote = mpBeats[from]->toVariantMap();

   int direction = ( from < to ) ? 1 : -1;
   int last = from;
   for( int i = from + direction; i != to + direction; i += direction )
   {
      mpBeats[last]->fromVariantMap( mpBeats[i]->toVariantMap() );
      last = i;
   }

   mpBeats[to]->fromVariantMap( saveNote );
}


PsmkBeatWidget *PsmkPatternWidget::beatWidgetAt( const QPoint &pos )
{
   PsmkBeatWidget *beatWidget = 0;
   for( QObject *o = childAt( pos ); o && (o != this); o = o->parent() )
   {
      beatWidget = qobject_cast<PsmkBeatWidget*>(o);
   }
   return beatWidget;
}


void PsmkPatternWidget::contextMenuEvent( QContextMenuEvent *event )
{
   PsmkContextMenu menu( tr("Pattern"), cMimeType, toVariantMap(), this );

   if( menu.runSelect( event->globalPos() - QPoint( 5, 5 ) ) == PsmkContextMenu::Paste )
   {
      fromVariantMap( menu.mimeData().toMap() );
   }
}
