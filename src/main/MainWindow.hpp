/**
 * MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

/* base class */
#include <QtGlobal>
#if QT_VERSION < 0x050000
#include <QMainWindow>
#else
#include <QtWidgets/QMainWindow>
#endif

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class MainWidget;


/*!
 \brief main window

*/
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
    \param flags
   */
   explicit MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief destructor

   */
   virtual ~MainWindow();

public slots:
   /*!
    \brief update the menus

   */
   void updateMenus();
   /*!
    \brief \todo

    \param enabled
   */
   void setClearSettingsOnExit( bool enabled );

protected:
   /*!
    \brief very ugly workaround for wrong position restoration on compiz-fusion

    \param event
    \return bool
   */
   virtual bool event( QEvent *event );

private:
   Q_DISABLE_COPY( MainWindow )

   int                  mForbidMove; /*!< \brief counter for compiz-fusion workaround */
   MainWidget           *mpMainWidget; /*!< \brief the main widget */
   bool                 mClearSettingsOnExit; /*!< \brief uninstall replacement */
};

#endif
