/**
 * SlocumSong.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SlocumSong.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */


SlocumSound::SlocumSound( const QString &setname )
: name( setname )
, type()
, atten()
{
}


SlocumSound::SlocumSound( const QVariantMap &variantMap )
: name()
, type()
, atten()
{
   fromVariantMap( variantMap );
}


SlocumSound::~SlocumSound()
{
}


SlocumSound &SlocumSound::operator=( const SlocumSound &that )
{
   for( int i = 0; i < numEntries; ++i )
   {
      type[i] = that.type[i];
      atten[i] = that.atten[i];
   }
   name = that.name;

   return *this;
}


bool SlocumSound::operator==( const SlocumSound &that ) const
{
   for( int i = 0; i < numEntries; ++i )
   {
      if( (type[i] != that.type[i] ) ||
          (atten[i] != that.atten[i] ) )
      {
         return false;
      }
   }
   return name == that.name;
}


bool SlocumSound::operator!=( const SlocumSound &that ) const
{
   return !(*this == that);
}


QVariantMap SlocumSound::toVariantMap() const
{
   QVariantMap variantMap;
   QVariantList sounds;

   for( int i = 0; i < numEntries; ++i )
   {
      QVariantMap sound;
      sound.insert( "type", type[i] );
      sound.insert( "atten", atten[i] );
      sounds.append( sound );
   }
   variantMap.insert( "name", name );
   variantMap.insert( "sounds", sounds );

   return variantMap;
}


void SlocumSound::fromVariantMap( const QVariantMap &variantMap )
{
   bool ok;
   Q_ASSERT( variantMap.contains( "name" ) );
   name = variantMap.value( "name" ).toString();

   Q_ASSERT( variantMap.contains( "sounds" ) );
   QVariantList sounds( variantMap.value( "sounds" ).toList() );
   Q_ASSERT( sounds.size() == numEntries );
   for( int i = 0; i < numEntries; ++i )
   {
      QVariantMap sound( sounds.at(i).toMap() );
      Q_ASSERT( sound.contains( "type" ) );
      type[i]  = sound.value( "type" ).toUInt( &ok );
      Q_ASSERT( ok );

      Q_ASSERT( sound.contains( "atten" ) );
      atten[i] = sound.value( "atten" ).toUInt( &ok );
      Q_ASSERT( ok );
   }
}


int SlocumSound::size()
{
   return numEntries;
}
