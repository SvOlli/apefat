/*
 * PlayerConfig.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PlayerConfig.hpp"

#include "QFile"

#include "System.hpp"
#include "TIASnd.hpp"


PlayerConfig::PlayerConfig( TIASound *tia )
: mKeepOnRunning( true )
, mpTIA( tia )
{
}


PlayerConfig::~PlayerConfig()
{
}


void PlayerConfig::invalid( BYTE opcode )
{
   switch( opcode )
   {
   case 0x02:
   case 0x12:
   case 0x22:
   case 0x32:
   case 0x42:
   case 0x52:
   case 0x62:
   case 0x72:
   case 0x92:
   case 0xb2:
   case 0xd2:
      break;
   case 0xf2:
      mKeepOnRunning = false;
      break;
   default:
      break;
   }
}


void PlayerConfig::reset()
{
   for( ADDRESS i = 0; i < 0x200; ++i )
   {
      mMemory[i] = 0;
   }
}


BYTE PlayerConfig::peek( ADDRESS addr )
{
   return mMemory[ addr & 0x1fff ];
}


void PlayerConfig::poke( ADDRESS addr, BYTE value )
{
   mMemory[ addr & 0x1fff ] = value;
   if( addr == 0x0000 )
   {
      mKeepOnRunning = false;
   }
   else if( (addr >= 0x0015) && (addr <= 0x001a) )
   {
      mpTIA->set( addr, value );
   }
}


bool PlayerConfig::keepOnRunning()
{
   bool retval = mKeepOnRunning;
   mKeepOnRunning = true;
   return retval;
}


void PlayerConfig::dumpMem()
{
   static int counter = 0;
   QFile tmp( QString("/tmp/apefat.%1.dump").arg(counter++) );
   tmp.open( QIODevice::WriteOnly );
   tmp.write( (const char*)&mMemory[0], 0x2000 );
   tmp.close();
}
