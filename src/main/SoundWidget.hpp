/**
 * SoundWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SOUNDWIDGET_HPP
#define SOUNDWIDGET_HPP SOUNDWIDGET_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QSpinBox;
class QSignalMapper;

/* forward declaration of local classes */
class SlocumSong;
class SoundComboBox;

/*!
 \brief widget for setting \ref SlocumSong

 Set the sound configuration for a song: 8 "instruments" using one combination
 of a type (AUDCx) with an attenuation.

*/
class SoundWidget : public QGroupBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong song to pull slocumSong->sound() from
    \param parent
   */
   explicit SoundWidget( SlocumSong *slocumSong, QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~SoundWidget();

public slots:
   /*!
    \brief set all text labels

    triggered internally by constructor, must be retriggered after translation
    changes

   */
   void setTexts();
   /*!
    \brief set the informal name of this combination

    \param setname
   */
   void setName( const QString &setname );
   /*!
    \brief handler for reading a changed type configuration

    \param index
   */
   void typeChanged( int index );
   /*!
    \brief handler for reading a changed attenuation configuration

    \param index
   */
   void attensChanged( int index );
   /*!
    \brief re-read data from song

    \param slocumSong pointer to SlocumSong to use
   */
   void setFromSong( SlocumSong *slocumSong );

private:
   Q_DISABLE_COPY( SoundWidget )

   SlocumSong              *mpSlocumSong; /*!< \brief the song data to configure */
   QSignalMapper           *mpTypesMapper; /*!< \brief mapper for \ref typeChanged */
   QSignalMapper           *mpAttensMapper; /*!< \brief napper for \ref attenChanged */
   QGridLayout             *mpMainLayout; /*!< \brief layout */
   QLabel                  *mpLabelName; /*!< \brief label for name */
   QLabel                  *mpLabelType; /*!< \brief label for type */
   QLabel                  *mpLabelAtten; /*!< \brief label for atten */
   QLineEdit               *mpValueName; /*!< \brief line edit for name */
   QList<SoundComboBox*>   mTypes; /*!< \brief edit widget for type */
   QList<QSpinBox*>        mAttens; /*!< \brief edit widgets for attenuations */
};

#endif // SOUNDWIDGET_HPP
