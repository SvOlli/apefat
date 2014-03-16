/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QBoxLayout>
#include <QCommonStyle>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QFileDialog>

/* local library headers */
#include <json.h>

/* local headers */
#include "PsmkMainWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkBeatWidget.hpp"

#include <QtDebug>

const char MainWidget::cFileExtension[] = ".psmk";


MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpLayout( new QGridLayout( this ) )
, mpPsmkSong( new PsmkMainWidget( this ) )
, mpBarsScrollArea( 0 )
{
   QSettings settings;
   QVariant songData( settings.value( "SongData") );
   if( songData.type() == QVariant::Map )
   {
      mpPsmkSong->fromVariantMap( songData.toMap() );
   }
   //mpSlocumSong->setDefaults();
   mpLayout->setContentsMargins( 0, 0, 0, 0 );

   setLayout( mpLayout );

   smallScreenMode( settings.value( "SmallScreenMode", false ).toBool() );
}


MainWidget::~MainWidget()
{
   QSettings().setValue( "SongData", mpPsmkSong->toVariantMap() );
}


void MainWidget::fileNew()
{
   fileLoad( QString(":/EmptySong.psmk") );
}


void MainWidget::fileOpen()
{
   QSettings settings;
   settings.beginGroup( "Paths" );
   QString songDir( settings.value("SongDirectory", QDir::homePath()).toString() );

   QString fileName( QFileDialog::getOpenFileName( this, tr("Open File:"), songDir,
                                                   tr("Paul Slocum Music Kit (*%1)").arg( cFileExtension ) ) );

   if( fileLoad( fileName ) )
   {
      settings.setValue( "SongDirectory", QFileInfo(fileName).absolutePath() );
   }
}


bool MainWidget::fileLoad( const QString &fileName )
{
   if( !fileName.isEmpty() )
   {
      QFile file( fileName );
      if( file.exists() )
      {
         if( file.open( QIODevice::ReadOnly ) )
         {
            setSongFromJson( file.readAll() );
            file.close();
            return true;
         }
      }
   }
   return false;
}


void MainWidget::fileSave()
{
   QSettings settings;
   settings.beginGroup( "Paths" );
   QString songDir( settings.value("SongDirectory", QDir::homePath()).toString() );

   QString fileName( QFileDialog::getSaveFileName( this, tr("Save File:"), songDir,
                                                   tr("Paul Slocum Music Kit (*%1)").arg( cFileExtension ) ) );

   if( !fileName.isEmpty() )
   {
      if( !fileName.endsWith( cFileExtension ) )
      {
         fileName.append( cFileExtension );
      }
      QFile file( fileName );
      if( file.open( QIODevice::WriteOnly ) )
      {
         QByteArray data( QtJson::serialize( mpPsmkSong->toVariantMap() ) );
         QByteArray saveData;
         int indention = 0;
         foreach( QChar c, data )
         {
            switch( c.toAscii() )
            {
#if 1
            case '{':
            case '[':
               saveData.append( c );
               saveData.append( '\n' );
               saveData.append( QByteArray( ++indention, ' ' ) );
               break;
            case ',':
               saveData.append( c );
               saveData.append( '\n' );
               saveData.append( QByteArray( indention, ' ' ) );
               break;
            case '}':
            case ']':
               --indention;
               // slip through
#else
               saveData.append( '\n' );
               saveData.append( QByteArray( ++indention, ' ' ) );
               saveData.append( c );
               saveData.append( '\n' );
               saveData.append( QByteArray( indention, ' ' ) );
               break;
            case ',':
               saveData.append( c );
               saveData.append( '\n' );
               saveData.append( QByteArray( indention, ' ' ) );
               break;
            case '}':
            case ']':
               saveData.append( '\n' );
               saveData.append( QByteArray( --indention, ' ' ) );
               saveData.append( c );
               break;
#endif
            default:
               saveData.append( c );
            }
         }
         file.write( saveData );
         file.close();
         settings.setValue( "SongDirectory", QFileInfo(fileName).absolutePath() );
      }
   }
}


void MainWidget::fileExport()
{
   QSettings settings;
   settings.beginGroup( "Paths" );
   QString songName( settings.value("ExportFile", QDir::homePath()).toString() );

   QString fileName( QFileDialog::getSaveFileName( this, tr("Export File:"), songName,
                                                   tr("All files(*)") ) );

   if( !fileName.isEmpty() )
   {
      QFile file( fileName );
      if( file.open( QIODevice::WriteOnly ) )
      {
         file.write( mpPsmkSong->toSourceCode() );
         file.close();
         settings.setValue( "ExportFile", fileName );
      }
   }
}


bool MainWidget::setSongFromJson( const QByteArray &data )
{
   bool ok = true;
   mpPsmkSong->fromVariantMap( QtJson::parse( QString::fromUtf8( data ), ok ).toMap() );
   return ok;
}


void MainWidget::smallScreenMode( bool enabled )
{
   // small screen workaround, can't do this with the designer :-P
   if( enabled )
   {
      if( !mpBarsScrollArea )
      {
         mpBarsScrollArea = new QScrollArea( this );
         mpBarsScrollArea->setAlignment( Qt::AlignCenter );
      }
      mpBarsScrollArea->setWidget( mpPsmkSong );
      mpLayout->addWidget( mpBarsScrollArea, 0, 0 );
   }
   else
   {
      if( mpBarsScrollArea )
      {
         mpBarsScrollArea->deleteLater();
         mpBarsScrollArea = 0;
      }
      mpLayout->addWidget( mpPsmkSong, 0, 0 );
   }
   QSettings().setValue( "SmallScreenMode", enabled );
}
