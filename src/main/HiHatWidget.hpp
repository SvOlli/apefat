/**
 * HiHatWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef HIHATWIDGET_HPP
#define HIHATWIDGET_HPP HIHATWIDGET_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QBoxLayout;
class QCheckBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QSignalMapper;

/* forward declaration of local classes */
class SlocumSong;
class SlocumHiHat;
class SoundComboBox;
class PitchComboBox;

/*!
 \brief widget for editing \ref SlocumHiHat

*/
class HiHatWidget : public QGroupBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param parent
   */
   explicit HiHatWidget( SlocumSong *slocumSong,  QWidget *parent = 0 );
   /*!
    \brief constructor

    Not used right now, intended for editing a hihat configuration outside of
    a song.

    \param slocumHiHat
    \param parent
   */
   explicit HiHatWidget( SlocumHiHat *slocumHiHat, QWidget *parent = 0 );
   /*!
    \brief destrutor

   */
   virtual ~HiHatWidget();

public slots:
   /*!
    \brief set the informal name

    \param setname
   */
   void setName( const QString &setname );
   /*!
    \brief set the start pattern

    a number between 0 and 255

    \param i
   */
   void setStart( int i );
   /*!
    \brief set the sound (AUDCx) of the hihat

    \param i
   */
   void setSound( quint8 i );
   /*!
    \brief set the pitch (AUDFx) of the hihat

    \param i
   */
   void setPitch( quint8 i );
   /*!
    \brief set the volume (AUDVx) of the hihat

    \param i
   */
   void setVolume( int i );
   /*!
    \brief handler for the 32 beats of the hihat pattern

    \param index
   */
   void patternChanged( int index );
   /*!
    \brief re-read data from SlocumSong

    \param slocumSong new pointer to set mpSlocumSong to
   */
   void setFromSong( SlocumSong *slocumSong );
   /*!
    \brief re-read data from SlocumHiHat

    for later use

    \param slocumHiHat new pointer to set mpSlocumSong to
   */
   void setFromSong( SlocumHiHat *slocumHiHat );
   /*!
    \brief set text of widgets

    internally used by constructor, must be re-called when translations change

   */
   void setTexts();

private:
   Q_DISABLE_COPY( HiHatWidget )

   /*!
    \brief internally used by constructors

   */
   void setup();
   /*!
    \brief return mpSlocumSong->hihat() or mpSlocumHiHat, according to configuration

    \return SlocumHiHat
   */
   SlocumHiHat *hihat();
   /*!
    \brief internally used by setFromSong with parameter

   */
   void setFromSong();

   SlocumSong        *mpSlocumSong; /*!< \brief song data to change */
   SlocumHiHat       *mpSlocumHiHat; /*!< \brief the hihat data to change (indicates out-of-song usage) */
   QSignalMapper     *mpPatternMapper; /*!< \brief mapper for checkboxes */
   QBoxLayout        *mpMainLayout; /*!< \brief main layout */
   QBoxLayout        *mpNameLayout; /*!< \brief sublayout for name */
   QBoxLayout        *mpNumbersLayout; /*!< \brief sublayout for sound configuration */
   QBoxLayout        *mpPatternLayout; /*!< \brief sublayout for hihat pattern */
   QLabel            *mpLabelName; /*!< \brief label for name */
   QLabel            *mpLabelStart; /*!< \brief label for start */
   QLabel            *mpLabelSound; /*!< \brief label for sound */
   QLabel            *mpLabelPitch; /*!< \brief label for pitch */
   QLabel            *mpLabelVolume; /*!< \brief label for volume */
   QLineEdit         *mpValueName; /*!< \brief value for name */
   QSpinBox          *mpValueStart; /*!< \brief value for start */
   SoundComboBox     *mpValueSound; /*!< \brief value for sound */
   PitchComboBox     *mpValuePitch; /*!< \brief value for pitch */
   QSpinBox          *mpValueVolume; /*!< \brief value for volume */
   QList<QCheckBox*> mPattern; /*!< \brief checkboxes for hihat pattern */
};

#endif // HIHATWIDGET_HPP
