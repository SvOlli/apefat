/**
 * SlocumSong.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMSONG_HPP
#define SLOCUMSONG_HPP SLOCUMSONG_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SlocumBarList.hpp"
#include "SlocumHiHat.hpp"
#include "SlocumSound.hpp"

/* forward declaration of Qt classes */
#include <QString>
#include <QVariantMap>

/* forward declaration of local classes */


/*!
 * \brief The SongBinary struct
 */
struct SongBinary
{
   int            songSize;
   int            highBeatSize;
   int            lowBeatSize;
   int            beatsSize;
   unsigned char  voice0[256];
   unsigned char  voice1[256];
   int            highBeatIndex[64][4];
   int            lowBeatIndex[64][4];
   unsigned char  beats[512][9];
   unsigned char  soundTypeArray[8];
   unsigned char  soundAttenArray[8];
   unsigned char  hatPattern[4];
   unsigned char  hatStart;
   unsigned char  hatVolume;
   unsigned char  hatPitch;
   unsigned char  hatSound;
   unsigned char  tempoDelay;
};


/*!
 \brief class holding the whole sound structure

 This class holds the whole song as it's played: repetitions done by repeating
 them in the data structure. The boiling down into sourcecode for inclusion is
 done by another class which also is responsible for recognising and eleminating
 the redundant SlocumBeat and SlocumBar instances.

 All component of the song are passed returned as pointers for direct data
 manipulation.

*/

class SlocumSong
{
public:
   /*!
    \brief constructor

    \param setname
   */
   explicit SlocumSong( const QString &setname = QString() );
   /*!
    \brief constructor for a serialized object

    \param variantMap
   */
   explicit SlocumSong( const QVariantMap &variantMap );
   /*!
    \brief destructor

   */
   virtual ~SlocumSong();

   /*!
    \brief pointer holding the informal name of the song

    \return QString
   */
   QString *name();
   /*!
    \brief pointer to the songs sound configuration

    \return SlocumSound
   */
   SlocumSound *sound();
   /*!
    \brief pointer to the songs sound configuration

    \return SlocumSound
   */
   const SlocumSound *sound() const;
   /*!
    \brief pointer to the songs hi hat configuration

    \return SlocumHiHat
   */
   SlocumHiHat *hihat();
   /*!
    \brief pointer to the songs hi hat configuration

    \return SlocumHiHat
   */
   SlocumBarList *voice( int i );

   /*!
    \brief serialize data

    \return QVariantMap
   */
   QVariantMap toVariantMap() const;
   /*!
    \brief deserialize data

    \param variantMap source data
    \return success
   */
   bool fromVariantMap( const QVariantMap &variantMap );

   /*!
    \brief return the delay (speed of song)

    \return int
   */
   int delay();
   /*!
    \brief set the delay (speed of song)

    \param delay
   */
   void setDelay( int delay );
   /*!
    \brief count the number of unique bars for informal purposes

    \param lowVolume true: count lowVolume, false: count highVolume pattern
    \return int
   */
   int countUniqueBars( bool lowVolume );
   /*!
    \brief return number of bars in a voice

    \param voice id of voice (0 or 1)
    \return int
   */
   int size( quint8 voice );
   /*!
    \brief make sure constrains for an editable song is met

    Make sure that the song contains at least one bar, and the number of bars
    for each voice is the same. This is done by removing empty bars at the end
    of the larger voice, or if this is not possible, by padding the shorter
    voice with empty bars.
   */
   void sanitize();
   /*!
    \brief \todo

    \param quint8
    \return QString
   */
   static QString toHex( quint8 value );
   /*!
    \brief \todo

    \param quint8
    \return QString
   */
   static QString toBinary( quint8 value );

   /*!
    \brief export song to source code

    \return QByteArray
   */
   QByteArray toSourceCode();

   /*!
    * \brief toSongBinary
    * \param songBinary
    */
   void toSongBinary( SongBinary *songBinary );

private:
   QString           mName; /*!< \brief informal name of the song */
   int               mDelay; /*!< \brief delay (speed) */
   SlocumSound       mSound; /*!< \brief sound configuration */
   SlocumHiHat       mHiHat; /*!< \brief hihat configuration */
   SlocumBarList     mVoice[2]; /*!< \brief both voice  */
   const QString     mSongType; /*!< \brief currently only "slocum0", intended to distinguish different formats */
};

#endif
