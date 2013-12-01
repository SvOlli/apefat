/**
 * SlocumHiHat.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SLOCUMHIHAT_HPP
#define SLOCUMHIHAT_HPP SLOCUMHIHAT_HPP

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
 \brief contains hihat configuration for song

 One per \ref SlocumSong. User interaction is done using
 \ref HiHatWidget.

*/
class SlocumHiHat
{
private:
   static const int numEntries = 32; /*!< \brief number of entries in hihat pattern */

public:
   /*!
    \brief constructor

    \param setname
   */
   SlocumHiHat( const QString &setname = QString() );
   /*!
    \brief constructor for deserializing on creation

    \param setname
   */
   SlocumHiHat( const QVariantMap &variantMap );
   /*!
    \brief destructor

   */
   virtual ~SlocumHiHat();

   /*!
    \brief assignment operator

    \param that
    \return SlocumHiHat
   */
   SlocumHiHat &operator=( const SlocumHiHat &that );
   /*!
    \brief check same operator

    \param that
    \return bool
   */
   bool operator==( const SlocumHiHat &that ) const;
   /*!
    \brief check different operator

    \param that
    \return bool
   */
   bool operator!=( const SlocumHiHat &that ) const;

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
    \brief return number of entries in hihat pattern

    \return int
   */
   static int size();

   QString  name; /*!< \brief informal name */
   quint8   start; /*!< \brief number of start pattern */
   quint8   volume; /*!< \brief volume of hihat (AUDVx value, 0-15) */
   quint8   sound; /*!< \brief sound of hihat (AUDCx value, 0-15) */
   quint8   pitch; /*!< \brief pitch of hihat (AUDFx value, 0-31) */
   bool     pattern[numEntries]; /*!< \brief array for hihat pattern */
};

#endif
