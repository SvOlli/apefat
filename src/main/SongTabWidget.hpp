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
class QLabel;
class QListView;
class QPushButton;
class QSpinBox;

/* forward declaration of local classes */
class BarWidget;
class HiHatWidget;
class SoundWidget;
class SlocumBar;
class SlocumSong;
class PlayerWidget;
class VoiceModel;
class VoiceWidget;

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

    \param slocumSong (0: reload current)
   */
   void setFromSong( SlocumSong *slocumSong = 0 );
   /*!
    \brief \todo

   */
   void cleanup();
   /*!
    \brief \todo

    \param value
   */
   void changeSliders( int value );
   /*!
    \brief \todo

    \param value
   */
   void setSongDelay( int value );

protected:

private:
   Q_DISABLE_COPY( SongTabWidget )

   SlocumSong           *mpSlocumSong; /*!< \brief song data to edit */
   HiHatWidget          *mpHiHatWidget; /*!< \brief hihat pattern */
   SoundWidget          *mpSoundWidget; /*!< \brief sound pattern */
   QLabel               *mpDelayName; /*!< \brief \todo */
   QSpinBox             *mpDelayValue; /*!< \brief \todo */
   QPushButton          *mpLinkButton; /*!< \brief \todo */
   PlayerWidget         *mpPlayerWidget; /*!< \brief \todo */
   QLabel               *mpUniqueInfo; /*!< \brief \todo */
   VoiceWidget          *mpVoice0; /*!< \brief voice editing */
   VoiceWidget          *mpVoice1; /*!< \brief voice editing */
};

#endif // SONGTABWIDGET_HPP
