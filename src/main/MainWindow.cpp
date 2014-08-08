/**
 * MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWindow.hpp"

/* system headers */

/* Qt headers */
#if QT_VERSION < 0x050000
#include <QApplication>
#include <QMenuBar>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#endif
#include <QSettings>

/* local library headers */

/* local headers */
#include "MainWidget.hpp"


MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mForbidMove( 50 )
, mpMainWidget( new MainWidget( this ) )
, mClearSettingsOnExit( false )
{
   QSettings settings;
   setCentralWidget( mpMainWidget );
   setWindowTitle( QApplication::applicationName()+
                   ": A Poor Excuse For A Tracker" );

   updateMenus();
   //setWindowIcon( QIcon( ":/clock.png" ) );
   restoreGeometry( settings.value("Geometry").toByteArray() );
   restoreState( settings.value("State").toByteArray() );
}


MainWindow::~MainWindow()
{
   QSettings settings;
   if( mClearSettingsOnExit )
   {
      settings.sync();
      settings.clear();
   }
   else
   {
      settings.setValue( "Geometry", saveGeometry() );
      settings.setValue( "State",    saveState() );
   }
}


void MainWindow::updateMenus()
{
   QSettings settings;
   QMenuBar *mainMenuBar = menuBar();
   if( mainMenuBar )
   {
      delete mainMenuBar;
   }
   mainMenuBar = new QMenuBar( this );
   mainMenuBar->setObjectName( "MainMenu" );
   //todo: move to setText()
   QMenu *fileMenu = mainMenuBar->addMenu( tr("&File") );
   QKeySequence quitSequence( QKeySequence::Quit );
   if( quitSequence.isEmpty() )
   {
      quitSequence = QKeySequence( tr("Ctrl+Q") );
   }
   fileMenu->setObjectName( "FileMenu" );
   fileMenu->addAction( tr("&New"),    mpMainWidget, SLOT(fileNew()) );
   fileMenu->addAction( tr("&Open"),   mpMainWidget, SLOT(fileOpen()),   QKeySequence( QKeySequence::Open ) );
   fileMenu->addAction( tr("&Save"),   mpMainWidget, SLOT(fileSave()),   QKeySequence( QKeySequence::Save ) );
   fileMenu->addAction( tr("&Export"), mpMainWidget, SLOT(fileExport()), QKeySequence( tr("Ctrl+E") ) );
   fileMenu->addAction( tr("E&xit"),   qApp,         SLOT(quit()),       quitSequence );
   QMenu *settingsMenu = mainMenuBar->addMenu( tr("&Settings") );
   QAction *action = settingsMenu->addAction( tr("&Small screen mode"), mpMainWidget, SLOT(smallScreenMode(bool)) );
   action->setCheckable( true );
   action->setChecked( settings.value( "SmallScreenMode", false ).toBool() );
   settingsMenu->addSeparator();
   action = settingsMenu->addAction( tr("&Clear settings on exit"), this, SLOT(setClearSettingsOnExit(bool)) );
   action->setCheckable( true );
   setMenuBar( mainMenuBar );
}


void MainWindow::setClearSettingsOnExit( bool enabled )
{
   mClearSettingsOnExit = enabled;
}


/* Evil workaround: in the first fifty events revert all move requests */
bool MainWindow::event( QEvent *event )
{
   if( mForbidMove > 0 )
   {
      if( event->type() == QEvent::Move )
      {
         restoreGeometry( QSettings().value("Geometry").toByteArray() );
      }
      --mForbidMove;
   }
   return QWidget::event( event );
}
