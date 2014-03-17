/**
 * PsmkContextMenu.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PsmkContextMenu.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QSettings>

/* local library headers */
#include <json.h>

/* local headers */


PsmkContextMenu::PsmkContextMenu( const QString &context,
                                  const char *mimeType,
                                  const QVariantMap &variantMap,
                                  QWidget *parent )
: QMenu( parent )
, mpTitle( addAction( context ) )
, mpSeparator1( addSeparator() )
, mpCopy( addAction( tr("Copy") ) )
, mpPaste( addAction( tr("Paste") ) )
, mpSeparator2( addSeparator() )
, mpStore( addAction( tr("Store") ) )
, mpLoad( addMenu( tr("Load") ) )
, mpRemove( addMenu( tr("Remove") ) )
, mpSettings( new QSettings( this ) )
, mpMimeType( mimeType )
, mName()
, mData( variantMap )
, mLoadList()
, mRemoveList()
{
   mpSettings->beginGroup( QString("Store/%1").arg( context ) );
   QStringList stored( mpSettings->childKeys() );

   if( stored.isEmpty() )
   {
      mpLoad->setEnabled( false );
      mpRemove->setEnabled( false );
   }
   else
   {
      foreach( const QString &entry, stored )
      {
         mLoadList.append( mpLoad->addAction( entry ) );
         mRemoveList.append( mpRemove->addAction( entry ) );
      }
   }

   if( variantMap.contains("name") )
   {
      mName = getNameFromMap( variantMap );
   }
   if( mName.isEmpty() )
   {
      mpStore->setEnabled( false );
   }

   mpTitle->setEnabled( false );
   if( !QApplication::clipboard()->mimeData()->hasFormat( mpMimeType ) )
   {
      mpPaste->setEnabled( false );
   }
}


PsmkContextMenu::~PsmkContextMenu()
{
}


PsmkContextMenu::SelectedAction PsmkContextMenu::runSelect( const QPoint &pos )
{
   QAction *selected = exec( pos );

   if( selected == mpCopy )
   {
      setMimeData( mData );
      return Copy;
   }
   else if( selected == mpPaste )
   {
      getMimeData();
      if( !mData.isNull() )
      {
         return Paste;
      }
   }
   else if( selected == mpStore )
   {
      mpSettings->setValue( mName, mData );
   }
   else if( mLoadList.contains( selected ) )
   {
      mData = mpSettings->value( selected->text() );
      return Paste;
   }
   else if( mRemoveList.contains( selected ) )
   {
      mpSettings->remove( selected->text() );
   }
   return None;
}


QVariant PsmkContextMenu::mimeData() const
{
   return mData;
}


void PsmkContextMenu::setMimeData( const QVariant &data )
{
   QClipboard *clipboard = QApplication::clipboard();
   QMimeData *mimeData = new QMimeData();
   mimeData->setData( mpMimeType, QtJson::serialize( data ) );
   mimeData->setText( QString::fromUtf8( mimeData->data( mpMimeType ) ) );
   clipboard->setMimeData( mimeData );
}


void PsmkContextMenu::getMimeData()
{
   const QMimeData *mimeData = QApplication::clipboard()->mimeData();
   if( mimeData->hasFormat( mpMimeType ) )
   {
      mData = QtJson::parse( QString::fromUtf8( mimeData->data( mpMimeType ) ) );
   }
   else
   {
      mData = QVariant();
   }
}


void PsmkContextMenu::removeStore()
{
   delete mpSeparator2;
   delete mpStore;
   delete mpLoad;
   delete mpRemove;
   mpSeparator2 = 0;
   mpStore      = 0;
   mpLoad       = 0;
   mpRemove     = 0;
}


QString PsmkContextMenu::getNameFromMap( const QVariantMap &variantMap, bool *success )
{
   if( success )
   {
      *success = variantMap.contains( "name" );
   }
   return variantMap.value( "name" ).toString();
}
