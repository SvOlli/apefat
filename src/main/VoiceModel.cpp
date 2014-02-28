/*
 * VoiceModel.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "VoiceModel.hpp"

/* system headers */

/* Qt headers */
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

/* local library headers */

/* local headers */
#include <json.h>


#include <QDebug>


VoiceModel::VoiceModel( SlocumSong *slocumSong, int voice, QObject *parent )
: QAbstractListModel( parent )
, mpSlocumSong( slocumSong )
, mVoice( voice )
{
}


VoiceModel::~VoiceModel()
{
}


int VoiceModel::rowCount( const QModelIndex &parent ) const
{
   if( parent.isValid() )
   {
      return 0;
   }

   return mpSlocumSong->size( mVoice );
}


Qt::ItemFlags VoiceModel::flags( const QModelIndex &index ) const
{
   /* \todo: set flags according to remote or local mode */
   if( !index.isValid() )
   {
      return Qt::ItemIsDropEnabled;
   }

   return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}


QVariant VoiceModel::data( const QModelIndex &index, int role ) const
{
   if( (index.row() < 0) || (index.row() >= mpSlocumSong->size( mVoice )) )
   {
      return QVariant();
   }

   const SlocumBar *bar = mpSlocumSong->voice( mVoice )->bar( index.row() );
   switch( role )
   {
   case Qt::DisplayRole:
   case Qt::EditRole:
      {
         QString text( QString("%1:%2").arg( QString::number(index.row()).rightJustified(3,'0'), bar->name ) );

         for( int i = 0; i < SlocumBar::size(); ++i )
         {
            text.append( QString("\n> %1").arg(bar->beat[i].name) );
         }
         return text;
      }
   case Qt::UserRole:
      {
         return bar->toVariantMap();
      }
   default:
      return QVariant();
   }
}


bool VoiceModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
   if( (index.row() >= 0) && (index.row() < mpSlocumSong->size( mVoice )) )
   {
      if( (role == Qt::EditRole) || (role == Qt::DisplayRole) )
      {
         qDebug() << __FILE__ << __LINE__ << Q_FUNC_INFO;
         return true;
      }
      else if( role == Qt::UserRole )
      {
         mpSlocumSong->voice( mVoice )->replace( index.row(), value.toMap() );
         emit dataChanged( index, index );
         return true;
      }
   }
   return false;
}


bool VoiceModel::insertRows( int row, int count, const QModelIndex &parent )
{
   if( (count < 1) || (row < 0) || (row > rowCount(parent)) )
   {
      return false;
   }

   beginInsertRows( QModelIndex(), row, row + count - 1 );

   for ( int i = 0; i < count; i++ )
   {
      mpSlocumSong->voice( mVoice )->insert( row, SlocumBar() );
   }

   endInsertRows();

   return true;
}


bool VoiceModel::removeRows( int row, int count, const QModelIndex &parent )
{
   if( (count <= 0) || (row < 0) || ((row + count) > rowCount(parent)) )
   {
      return false;
   }

   beginRemoveRows( QModelIndex(), row, row + count - 1 );

   for( int i = 0; i < count; i++ )
   {
      mpSlocumSong->voice( mVoice )->removeAt( row );
   }

   endRemoveRows();

   return true;
}


void VoiceModel::setSlocumSong( SlocumSong *slocumSong )
{
   beginResetModel();
   mpSlocumSong = slocumSong;
   endResetModel();
}


Qt::DropActions VoiceModel::supportedDragActions() const
{
   return Qt::MoveAction | Qt::CopyAction;
}


Qt::DropActions VoiceModel::supportedDropActions() const
{
   return Qt::MoveAction | Qt::CopyAction;
}


QStringList VoiceModel::mimeTypes() const
{
   QStringList types;
   types << "x-apefat-bars";
   return types;
}


QMimeData *VoiceModel::mimeData( const QModelIndexList &indexes ) const
{
   QMimeData *mimeData = new QMimeData();

   SlocumBarList list;
   QStringList mimeText;
   foreach( const QModelIndex &index, indexes )
   {
      if( index.isValid() )
      {
         mimeText.append( data( index, Qt::DisplayRole ).toString() );
         list.append( SlocumBar( data( index, Qt::UserRole ).toMap() ) );
      }
   }
   mimeData->setText( mimeText.join("\n") );
   mimeData->setData( mimeTypes().at(0), QtJson::serialize( list.toVariantList() ) );

   return mimeData;
}


bool VoiceModel::dropMimeData( const QMimeData *data, Qt::DropAction action,
                               int row, int column, const QModelIndex &parent )
{
   if( action == Qt::IgnoreAction )
   {
      return true;
   }

   if( column > 0 )
   {
      return false;
   }

   int beginRow;
   if( row != -1 )
   {
      beginRow = row;
   }
   else if( parent.isValid() )
   {
      beginRow = parent.row();
   }
   else
   {
      beginRow = rowCount();
   }

   QModelIndex idx;
   SlocumBarList list( QtJson::parse( QString::fromUtf8(data->data("x-apefat-bars")) ).toList() );
   insertRows( beginRow, list.size() );
   for( int i = 0; i < list.size(); i++ )
   {
      idx = index( beginRow + i );
      setData( idx, list.at(i).toVariantMap(), Qt::UserRole );
   }

   return true;
}
