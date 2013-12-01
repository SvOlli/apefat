/**
 * SongSound.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SONGSOUND_HPP
#define SONGSOUND_HPP SONGSOUND_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
#include <QString>
#include <QVariant>

/* forward declaration of local classes */

/*!
 \brief contains sound configuration for song

 One per \ref SlocumSong. User interaction is done using
 \ref SoundWidget.

*/
class SlocumSound
{
private:
   static const int numEntries = 8; /*!< \brief number of different tones in table */

public:
   /*!
    \brief constructor

    \param setname
   */
   SlocumSound( const QString &setname = QString() );
   /*!
    \brief constructor for deserializing on creation

    \param setname
   */
   SlocumSound( const QVariantMap &variantMap );
   /*!
    \brief destructor

   */
   virtual ~SlocumSound();

   /*!
    \brief assignment operator

    \param that
    \return SongSound
   */
   SlocumSound &operator=( const SlocumSound &that );
   /*!
    \brief check same operator

    \param that
    \return bool
   */
   bool operator==( const SlocumSound &that ) const;
   /*!
    \brief check different operator

    \param that
    \return bool operator
   */
   bool operator!=( const SlocumSound &that ) const;

   /*!
    \brief serialize data

    \return QVariantMap
   */
   QVariantMap toVariantMap() const;
   /*!
    \brief deserialize data

    \param variantMap
   */
   void fromVariantMap( const QVariantMap &variantMap );

   /*!
    \brief number of entries in table

    \return int
   */
   static int size();

   QString     name; /*!< \brief informal name */
   quint8      type[numEntries]; /*!< \brief sound type table */
   quint8      atten[numEntries]; /*!< \brief sound attenuation table */
};

#endif
