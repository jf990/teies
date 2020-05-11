/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_pack()                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    August 03, 1988                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   packet      string *   INOUT  Where to store result.   |
  |  :PARAM.   ocd         OCD    *   INPUT  What to pack.            |
  |                                                                   |
  |  :PURPOSE. Packetize the OCD as a string.  Format:                |
  |                                                                   |
  |   +---------+-----------+--------------+-----------+-----+        |
  |   | Offset  | Component | Number of    | Component |     |        |
  |   | 4 bytes | Name      | Keys         | Key       | ... |        |
  |   | (tint)  | ocd->name | ocd->num_keys| ocd->key  |     |        |
  |   +---------+-----------+--------------+-----------+-----+        |
  |                                                                   |
  |            Component name and keys are TEIES strings where the    |
  |            used and allocated lengths are in the packet corrected |
  |            to even allocations.  The offset and number of keys    |
  |            are both 4 byte integers.                              |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   ocd_pack(  packet, ocd  )

string           *packet;
OCD              *ocd;

{

   sint    rc           =    0,
           i            =    0;






if( OCD_POINTER_OK( ocd ) && packet != NIL )
{

   s_catb( packet, (byte *) & ( ocd->bias     ) ,     4 );
   s_catx( packet,              ocd->name       ,  0, 0 );
   s_catb( packet, (byte *) & ( ocd->num_keys ) ,     4 );

   if( ocd->num_keys > 1 )
   {

      for( i=1; i<=s_len( ocd->key ); i++ )
      {

         s_catx( packet, ocd->key + i, 0, 0 );

      }
   }
   else
   {

      s_catx( packet, ocd->key, 0, 0 );

   }
}
else
{

    rc = INVALID_DATA_POINTER;

}

   return(rc);

}
