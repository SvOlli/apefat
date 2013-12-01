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
class BarWidget;
class BarsTabWidget;
class HiHatWidget;
class SlocumSong;
class SongTabWidget;
class SoundWidget;


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
    \brief open a file open dialog and load selected file

   */
   void fileOpen();
   /*!
    \brief open a file save dialog and save data to selected file

   */
   void fileSave();
   /*!
    \brief set the current tab

    \param index
   */
   void setTab( int index = 0 );

   /*!
    \brief set the song data from Json serialized data

    \param data serialized json data
    \return data was valid
   */
   bool setSongFromJson( const QByteArray &data );

private:
   Q_DISABLE_COPY( MainWidget )

   static const char    cFileExtension[]; /*!< \brief file extension */

   SlocumSong           *mpSlocumSong; /*!< \brief song data */
   int                  mCurrentBar; /*!< \brief index of current bar */
   QTabWidget           *mpTabs; /*!< \brief tabs */
   SongTabWidget        *mpSongTab;
   BarsTabWidget        *mpBarsTab;
};

#endif // MAINWIDGET_HPP
