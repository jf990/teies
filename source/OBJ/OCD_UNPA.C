/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_unpack()                                           |
  |  <AUTHOR>  Tanmay S. Kumar                                        |
  |  <DATE>    August 12, 1988                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   packet      string *   INPUT  Data to unpack           |
  |  <PARAM>   ocd         OCD    **  INPUT  Where to put it.         |
  |                                                                   |
  |  <PURPOSE> Unpacketize the OCD as a string.  Format:              |
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
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   ocd_unpack(  packet, ocd  )

string              packet;
OCD               **ocd;

{

   sint rc = 0;
   byte * current_ptr = NIL;
   string curpointer = S_NULL;



if( packet != NIL )
{



 ocd_new(  ocd, NIL );
 current_ptr = s_buffer( packet );
 (*ocd)->bias = * ( tint * ) current_ptr;

 current_ptr += 4;
 s_copy( & ( (*ocd)->name ), (string)(current_ptr), 0, 0 );
 curpointer = (string)(current_ptr);

 current_ptr += ( s_smp_al(curpointer) ) + S_OVERHEAD;
 (*ocd)->num_keys = * (sint *)(current_ptr);

 current_ptr += 4;                          /* Loop for num of keys */
 curpointer = (string)(current_ptr);
 s_copy( & ( (*ocd)->key ), (string)(current_ptr), 0, 0 );

}


   return(rc);

}
