/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_rcvseg()                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 23, 1989                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   server      string     INPUT  description              |
  |  <PARAM>   packet      string     INPUT  description              |
  |  <PURPOSE> A server has send us something packed in obj_sndseg    |
  |            format, unpack it and call obj_replace with the result.|
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   obj_rcvseg( server, packet )

string             server, packet;


{

   sint        rc         =      0;
   OCB       * object     =    NIL;
   OCD       * component  =    NIL;



if( s_len( packet ) > 8 )
{

   object = obj_opn( (OID *) s_buffer( packet ), OBJ_UPDATE );
   if( OCB_POINTER_OK( object ) )
   {

      rc = ocd_unpack( (string) (s_buffer( packet ) + sizeof( OID )),
                                                           & component );
      if( rc == 0 )
      {

         rc = obj_replace_segment( object, component, (string)
                     (s_buffer( packet ) + sizeof( OID ) + S_OVERHEAD +
                 s_len( (string) (s_buffer( packet ) + sizeof( OID )))));

#if 0

/* Send a confirmation of the operation is not currently done */

         t_csend( server, rc, S_NULL );

#endif

      }
      ocd_drp( & component );
      obj_cls(   object    );

   }
   else
   {

      rc = OID_NOT_IN_SOIDI;

   }
}
else
{

  rc = INVALID_DATA_PACKET;

}


   return(rc);

}
