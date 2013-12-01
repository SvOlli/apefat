/**
 * SlocumHiHat.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumHiHat.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


SlocumHiHat::SlocumHiHat( const QString &setname )
: name( setname )
, start( 0 )
, volume( 0 )
, sound( 0 )
, pitch( 0 )
, pattern()
{
}


SlocumHiHat::SlocumHiHat( const QVariantMap &variantMap )
: name()
, start( 0 )
, volume( 0 )
, sound( 0 )
, pitch( 0 )
, pattern()
{
   fromVariantMap( variantMap );
}


SlocumHiHat::~SlocumHiHat()
{
}


SlocumHiHat &SlocumHiHat::operator=( const SlocumHiHat &that )
{
   start  = that.start;
   volume = that.volume;
   sound  = that.sound;
   pitch  = that.pitch;
   for( int i = 0; i < numEntries; ++i )
   {
      pattern[i]  = that.pattern[i];
   }
   name = that.name;

   return *this;
}


bool SlocumHiHat::operator==( const SlocumHiHat &that ) const
{
   for( int i = 0; i < numEntries; ++i )
   {
      if( pattern[i] != that.pattern[i] )
      {
         return false;
      }
   }
   return (start == that.start) && (volume  == that.volume) &&
          (sound == that.sound) && (pitch == that.pitch) &&
          (name == that.name);
}


bool SlocumHiHat::operator!=( const SlocumHiHat &that ) const
{
   return !(*this == that);
}


QVariantMap SlocumHiHat::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList patterns;

   for( int i = 0; i < numEntries; ++i )
   {
      patterns.append( pattern[i] );
   }
   variantMap.insert( "name", name );
   variantMap.insert( "start", start );
   variantMap.insert( "volume", volume );
   variantMap.insert( "sound", sound );
   variantMap.insert( "pitch", pitch );
   variantMap.insert( "patterns", patterns );

   return variantMap;
}


void SlocumHiHat::fromVariantMap( const QVariantMap &variantMap )
{
   bool ok;
   Q_ASSERT( variantMap.contains( "name" ) );
   name = variantMap.value( "name" ).toString();

   Q_ASSERT( variantMap.contains( "start" ) );
   start = variantMap.value( "start" ).toUInt( &ok );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains( "volume" ) );
   volume = variantMap.value( "volume" ).toUInt( &ok );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains( "sound" ) );
   sound = variantMap.value( "sound" ).toUInt( &ok );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains( "pitch" ) );
   pitch = variantMap.value( "pitch" ).toUInt( &ok );
   Q_ASSERT( ok );

   Q_ASSERT( variantMap.contains( "patterns" ) );
   QVariantList patterns( variantMap.value("patterns").toList() );
   Q_ASSERT( patterns.size() == numEntries );

   for( int i = 0; i < numEntries; ++i )
   {
      pattern[i] = patterns.at(i).toBool();
   }
}


int SlocumHiHat::size()
{
   return numEntries;
}
