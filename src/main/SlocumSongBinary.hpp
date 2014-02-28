/**
 * SlocumSongBinary.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMSONGBINARY_HPP
#define SLOCUMSONGBINARY_HPP SLOCUMSONGBINARY_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "SlocumSong.hpp"

/* forward declaration of Qt classes */
#include <QString>
#include <QVariantMap>

/* forward declaration of local classes */


/*!
 \brief class holding binary representation of the song

*/

class SlocumSongBinary
{
public:
   /*!
    \brief constructor

    \param setname
   */
   explicit SlocumSongBinary();
   /*!
    \brief destructor

   */
   virtual ~SlocumSongBinary();

   void clear();

   int                  songSize;
   int                  lowPatternsSize;
   int                  highPatternsSize;
   quint8               song1[256];
   quint8               song2[256];
   quint8               lowPatterns[64][9];
   quint8               highPatterns[64][9];
   quint8               soundTypeArray[8];
   quint8               soundAttenArray[8];
   quint8               hatPattern[4];
   quint8               hatStart;
   quint8               hatVolume;
   quint8               hatPitch;
   quint8               hatSound;
   quint8               tempoDelay;
};

#endif
