/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_recv()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    December 15, 1988                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT  Object Control Block of  |
  |                                          object to put components |
  |                                          into from the packet.    |
  |  <PARAM>   packet      string     INPUT  The packet to use for the|
  |                                          unpacking.               |
  |                                                                   |
  |  <PURPOSE> A packet is given that is assumed to be in DMS_PAK()   |
  |            format of a single data base path.  Unpack it into the |
  |            requested object.                                      |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |                                                                   |
  |  <ERRORS>  None known                                             |
  |                                                                   |
  |  <LIMITS>  None known                                             |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   ocd_recv(  object, packet  )

OCB             * object;
string            packet;

{

   sint        rc         =      0;





if( OCB_POINTER_OK( object ) )
{

   if( s_len( packet ) > 0 )
   {

      rc = dms_unpk( object->dmscb, s_buffer( packet ), s_len( packet ),
                                                                  NIL );
      if( rc == DMS_UNPACK_OK )
      {

         rc = 0;

      }
   }
   else
   {

      rc = NO_DATA_FOUND;

   }
}
else
{

   rc = INVALID_OCB;

}


   return(rc);

}
