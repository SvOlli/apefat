/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
class QGridLayout;
class QLabel;
class QPushButton;
class QScrollArea;
class QTabWidget;
class QTextEdit;

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class PlayerEmulation;
class PsmkMainWidget;


/*!
 \brief the main widget containing everything except the menu bar

*/
class MainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief contructor

    \param parent
   */
   explicit MainWidget( QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~MainWidget();

public slots:
   /*!
    \brief create an empty song

   */
   void fileNew();
   /*!
    \brief open a file open dialog and load selected file

   */
   void fileOpen();
   /*!
    \brief open a file save dialog and save data to selected file

   */
   void fileSave();
   /*!
    \brief open a file save dialog and export data in sourcecode format to selected file

   */
   void fileExport();
   /*!
    \brief set the song data from Json serialized data

    \param data serialized json data
    \return data was valid
   */
   bool setSongFromJson( const QByteArray &data );
   /*!
    \brief put in scroll areas for large widgets

    \param enabled
    \return bool
   */
   void smallScreenMode( bool enabled );

private:
   Q_DISABLE_COPY( MainWidget )

   bool fileLoad( const QString &fileName );

   static const char    cFileExtension[]; /*!< \brief file extension */

   QGridLayout          *mpLayout;
   PsmkMainWidget       *mpPsmkSong;
   QScrollArea          *mpBarsScrollArea; /*!< \brief scroll area for the bars tabs */
};

#endif // MAINWIDGET_HPP
