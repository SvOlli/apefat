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
#include "HiHatWidget.hpp"
#include "BarWidget.hpp"
#include "BarsTabWidget.hpp"
#include "SlocumSong.hpp"
#include "SongTabWidget.hpp"
#include "SoundWidget.hpp"


#include <QtDebug>

const char MainWidget::cFileExtension[] = ".psmk";


MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpSlocumSong( new SlocumSong() )
, mCurrentBar( 0 )
, mpTabs( new QTabWidget( this ) )
, mpSongTab( new SongTabWidget( mpSlocumSong, this ) )
, mpBarsTab( new BarsTabWidget( mpSlocumSong, this ) )
, mpBarsScrollArea( 0 )
{
   QSettings settings;
   QVariant songData( settings.value( "SongData") );
   if( songData.type() == QVariant::Map )
   {
      mpSlocumSong->fromVariantMap( songData.toMap() );
      mpSongTab->setFromSong( mpSlocumSong );
      mpBarsTab->setFromSong( mpSlocumSong );
      mpBarsTab->setBar( 0 );
   }
   QBoxLayout *mainLayout = new QVBoxLayout( this );

   //mpSlocumSong->setDefaults();
   mainLayout->setContentsMargins( 0, 0, 0, 0 );

   setLayout( mainLayout );
   mainLayout->addWidget( mpTabs );
   mpTabs->addTab( mpSongTab, tr("Song") ); //!< \todo move to setText()
   smallScreenMode( settings.value( "SmallScreenMode", false ).toBool() );
   connect( mpTabs, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)) );
   tabChanged( 0 );
}


MainWidget::~MainWidget()
{
   QSettings().setValue( "SongData", mpSlocumSong->toVariantMap() );
   delete mpSlocumSong;
}


void MainWidget::fileOpen()
{
   QSettings settings;
   settings.beginGroup( "Paths" );
   QString songDir( settings.value("SongDirectory", QDir::homePath()).toString() );

   QString fileName( QFileDialog::getOpenFileName( this, tr("Open File:"), songDir,
                                                   tr("Paul Slocum Music Kit (*%1)").arg( cFileExtension ) ) );

   if( !fileName.isEmpty() )
   {
      QFile file( fileName );
      if( file.exists() )
      {
         if( file.open( QIODevice::ReadOnly ) )
         {
            setSongFromJson( file.readAll() );
            file.close();
            settings.setValue( "SongDirectory", QFileInfo(fileName).absolutePath() );
         }
      }
   }
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
         QByteArray data( QtJson::serialize( mpSlocumSong->toVariantMap() ) );
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
         file.write( mpSlocumSong->toSourceCode() );
         file.close();
         settings.setValue( "ExportFile", fileName );
      }
   }
}


void MainWidget::setTab( int index )
{
   mpTabs->setCurrentIndex( index );
}


bool MainWidget::setSongFromJson( const QByteArray &data )
{
   bool ok = true;
   mpSlocumSong->fromVariantMap( QtJson::parse( QString::fromUtf8( data ), ok ).toMap() );
   mpSongTab->setFromSong( mpSlocumSong );
   mpBarsTab->setFromSong( mpSlocumSong );
   mpBarsTab->setBar( 0 );
   return ok;
}


void MainWidget::smallScreenMode( bool enabled )
{
   int index = mpTabs->currentIndex();
   // small screen workaround, can't do this with the designer :-P
   if( enabled )
   {
      mpTabs->removeTab( 1 );
      if( !mpBarsScrollArea )
      {
         mpBarsScrollArea = new QScrollArea( this );
         mpBarsScrollArea->setAlignment( Qt::AlignCenter );
      }
      mpBarsScrollArea->setWidget( mpBarsTab );
      mpTabs->addTab( mpBarsScrollArea, tr("Bar") );
   }
   else
   {
      mpTabs->removeTab( 1 );
      mpTabs->addTab( mpBarsTab, tr("Bar") );
      if( mpBarsScrollArea )
      {
         mpBarsScrollArea->deleteLater();
         mpBarsScrollArea = 0;
      }
   }
   mpTabs->setCurrentIndex( index );
   QSettings().setValue( "SmallScreenMode", enabled );
}


void MainWidget::tabChanged( int tab )
{
   if( tab == 0 )
   {
      mpSongTab->setTexts();
   }
}
