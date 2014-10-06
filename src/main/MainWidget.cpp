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
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QBoxLayout>
#include <QCommonStyle>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QTextEdit>
#include <QFileDialog>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QFileDialog>
#endif
#include <QSettings>
#include <QTimer>

/* local library headers */
#include <json.h>

/* local headers */
#include "PsmkSongWidget.hpp"
#include "PsmkInstrumentsWidget.hpp"
#include "PsmkBeatWidget.hpp"

#include <QtDebug>

const char MainWidget::cFileExtension[] = ".psmk";


MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpLayout( new QGridLayout( this ) )
, mpPsmkSong( new PsmkSongWidget( this ) )
, mpGlobalScrollArea( 0 )
{
   QSettings settings;

   fileLoad( QString(":/EmptySong.psmk") );
   mpLayout->setContentsMargins( 0, 0, 0, 0 );

   setLayout( mpLayout );

   smallScreenMode( settings.value( "SmallScreenMode", false ).toBool() );
}


MainWidget::~MainWidget()
{
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
            switch( c.toLatin1() )
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
      if( !mpGlobalScrollArea )
      {
         mpGlobalScrollArea = new QScrollArea( this );
         mpGlobalScrollArea->setAlignment( Qt::AlignCenter );
      }
      mpGlobalScrollArea->setWidget( mpPsmkSong );
      mpLayout->addWidget( mpGlobalScrollArea, 0, 0 );
   }
   else
   {
      if( mpGlobalScrollArea )
      {
         mpGlobalScrollArea->deleteLater();
         mpGlobalScrollArea = 0;
      }
      mpLayout->addWidget( mpPsmkSong, 0, 0 );
   }
   QSettings().setValue( "SmallScreenMode", enabled );
}
