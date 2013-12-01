/**
 * SlocumNote.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumNote.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

#include <QtDebug>

SlocumNote::SlocumNote()
: sound( 255 )
, pitch( 255 )
, accent( false )
{
}


SlocumNote::SlocumNote( const QVariantMap &variantMap )
: sound( 255 )
, pitch( 255 )
, accent( false )
{
   fromVariantMap( variantMap );
}


SlocumNote::~SlocumNote()
{
}


SlocumNote &SlocumNote::operator=( const SlocumNote &that )
{
   sound = that.sound;
   pitch = that.pitch;
   accent = that.accent;

   return *this;
}


bool SlocumNote::operator==( const SlocumNote &that ) const
{
   return ( sound  == that.sound )
       && ( pitch  == that.pitch )
       && ( accent == that.accent );
}


bool SlocumNote::operator!=( const SlocumNote &that ) const
{
   return !(*this == that);
}


bool SlocumNote::isEmpty()
{
   return sound == 255;
}


QVariantMap SlocumNote::toVariantMap() const
{
   QVariantMap variantMap;

   variantMap.insert( "sound",  sound );
   variantMap.insert( "pitch",  pitch );
   variantMap.insert( "accent", accent );

   return variantMap;
}


void SlocumNote::fromVariantMap( const QVariantMap &variantMap )
{
   bool ok;

   Q_ASSERT( variantMap.contains( "sound" ) );
   sound = variantMap.value( "sound" ).toUInt( &ok );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains("pitch") );
   pitch = variantMap.value( "pitch" ).toUInt( &ok );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains("accent") );
   accent = variantMap.value( "accent" ).toBool();
}
