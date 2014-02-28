/*
 * VoiceWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "VoiceWidget.hpp"

/* system headers */

/* Qt headers */
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMenu>
#include <QTimer>

/* local library headers */
#include "Trace.hpp"

/* local headers */
#include "VoiceModel.hpp"


VoiceWidget::VoiceWidget( SlocumSong *slocumSong, int voice, QWidget *parent )
: QListView( parent )
, mpSlocumSong( slocumSong )
, mpVoiceModel( new VoiceModel( slocumSong, voice, this ) )
{
   setEditTriggers( QAbstractItemView::NoEditTriggers );
   setMouseTracking( true );
   setAlternatingRowColors( true );
   setSelectionMode( QAbstractItemView::ExtendedSelection );
   setDragEnabled( true );
   setAcceptDrops( true );
   setDropIndicatorShown( true );
   viewport()->setAcceptDrops( true );
   setModel( mpVoiceModel );
   setContextMenuPolicy( Qt::ActionsContextMenu );
   setDragDropMode( QAbstractItemView::DragDrop );

   connect( mpVoiceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SIGNAL(sizeChanged()) );
   connect( mpVoiceModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SIGNAL(sizeChanged()) );
   setText();
}


VoiceWidget::~VoiceWidget()
{
}


void VoiceWidget::setFromSong( SlocumSong *slocumSong )
{
   mpSlocumSong = slocumSong;
   mpVoiceModel->setSlocumSong( mpSlocumSong );
}


int VoiceWidget::count()
{
   return mpVoiceModel->rowCount();
}


bool VoiceWidget::setCount( int count )
{
   while( count > mpVoiceModel->rowCount() )
   {
      int nextRow = mpVoiceModel->rowCount();
      mpVoiceModel->insertRow( nextRow );
   }
   while( count < mpVoiceModel->rowCount() )
   {
      int row = mpVoiceModel->rowCount() - 1;
      SlocumBar bar( mpVoiceModel->data( mpVoiceModel->index( row ), Qt::UserRole).toMap() );
      if( bar.isEmpty() )
      {
         mpVoiceModel->removeRow( row );
      }
      else
      {
         /* should not remove valid data */
         return false;
      }
   }
   return true;
}


void VoiceWidget::setText()
{
   QList<QAction*> actionList( actions() );
   while( actionList.size() )
   {
      delete actionList.takeLast();
   }
   QAction *action = 0;
   QKeySequence shortcut;
   action = new QAction( tr("Copy"), this );
   shortcut = QKeySequence( QKeySequence::Copy );
   grabShortcut( shortcut, Qt::WidgetShortcut );
   action->setShortcut( shortcut );
   connect( action, SIGNAL(triggered()),
            this, SLOT(copy()) );
   addAction( action );

   action = new QAction( tr("Cut"), this );
   shortcut = QKeySequence( QKeySequence::Cut );
   grabShortcut( shortcut, Qt::WidgetShortcut );
   action->setShortcut( shortcut );
   connect( action, SIGNAL(triggered()),
            this, SLOT(cut()) );
   addAction( action );

   action = new QAction( tr("Paste"), this );
   shortcut = QKeySequence( QKeySequence::Paste );
   grabShortcut( shortcut, Qt::WidgetShortcut );
   action->setShortcut( shortcut );
   connect( action, SIGNAL(triggered()),
            this, SLOT(paste()) );
   addAction( action );
}


void VoiceWidget::copy()
{
   QMimeData *mimeData = mpVoiceModel->mimeData( selectedIndexes() );
   QApplication::clipboard()->setMimeData( mimeData );

   qDebug() << "copy";
}


void VoiceWidget::cut()
{
   QModelIndexList list( selectedIndexes() );
   QMimeData *mimeData = mpVoiceModel->mimeData( list );
   QApplication::clipboard()->setMimeData( mimeData );
   while( !list.isEmpty() )
   {
      mpVoiceModel->removeRow( list.takeLast().row() );
   }
   qDebug() << "cut";
}


void VoiceWidget::paste()
{
   QModelIndex index( currentIndex() );
   const QMimeData *mimeData = QApplication::clipboard()->mimeData();
   if( mimeData->hasFormat( mpVoiceModel->mimeTypes().at(0) ) )
   {
      mpVoiceModel->dropMimeData( mimeData, Qt::CopyAction, index.row(), 0, index );
   }
   qDebug() << "paste";
}


void VoiceWidget::contextMenuEvent( QContextMenuEvent *event )
{
   QMenu menu( this );
   menu.addActions( actions() );
   QAction *selected = menu.exec( event->globalPos(), 0 );
   if( selected )
   {
      selected->trigger();
   }
}

void VoiceWidget::keyPressEvent( QKeyEvent *event )
{
   QList<QAction*> list( actions() );
   foreach( QAction *action, list )
   {
      qDebug() << event->text() << action->shortcut().toString();
      if( event->text() == action->shortcut().toString() )
      {
         action->trigger();
         event->accept();
         return;
      }
   }

   QListView::keyPressEvent( event );
}
