/**
 * SongTabWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SONGTABWIDGET_HPP
#define SONGTABWIDGET_HPP SONGTABWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QScrollArea;

/* forward declaration of local classes */
class BarWidget;
class HiHatWidget;
class SoundWidget;
class SlocumBar;
class SlocumSong;

/*!
 \brief representing the tab "Song"

 This widget contains the song configuration for sound, hihat and order of
 patterns.

*/
class SongTabWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param parent
   */
   explicit SongTabWidget( SlocumSong *slocumSong, QWidget *parent = 0 );
   /*!
    \brief destuctor

   */
   virtual ~SongTabWidget();

public slots:
   /*!
    \brief set all translatable texts

   */
   void setTexts();
   /*!
    \brief re-load configuration from song data

    \param slocumSong
   */
   void setFromSong( const SlocumSong *slocumSong );

protected:

private:
   Q_DISABLE_COPY( SongTabWidget )

   SlocumSong           *mpSlocumSong; /*!< \brief song data to edit */
   HiHatWidget          *mpHiHatWidget; /*!< \brief hihat pattern */
   SoundWidget          *mpSoundWidget; /*!< \brief sound pattern */
   QScrollArea          *mpChannels; /*!< \brief container for both channels */
};

#endif // SONGTABWIDGET_HPP
