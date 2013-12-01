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
#include <QtGui>

/* local library headers */

/* local headers */
#include "MainWidget.hpp"


MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mForbidMove( 50 )
, mpMainWidget( new MainWidget( this ) )
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
}


void MainWindow::updateMenus()
{
   QMenuBar *mainMenuBar = menuBar();
   if( mainMenuBar )
   {
      delete mainMenuBar;
   }
   mainMenuBar = new QMenuBar( this );
   mainMenuBar->setObjectName( "MainMenu" );
   //todo: move to setText()
   QMenu *fileMenu = mainMenuBar->addMenu( tr("&File") );
   fileMenu->setObjectName( "FileMenu" );
   fileMenu->addAction( tr("&Open"), mpMainWidget, SLOT(fileOpen()), QKeySequence( tr("Ctrl+O") ) );
   fileMenu->addAction( tr("&Save"), mpMainWidget, SLOT(fileSave()), QKeySequence( tr("Ctrl+S") ) );
   fileMenu->addAction( tr("E&xit"), qApp,         SLOT(quit()),     QKeySequence( tr("Ctrl+Q") ) );
   setMenuBar( mainMenuBar );
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


void MainWindow::closeEvent( QCloseEvent *event )
{
   QSettings settings;
   settings.setValue( "Geometry", saveGeometry() );
   settings.setValue( "State",    saveState() );

   event->accept();
}
