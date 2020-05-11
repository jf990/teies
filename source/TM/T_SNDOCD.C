/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_sndocd()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    12/27/88                                               |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server      string     INPUT                           |
  |  <PARAM>   packet      string     INPUT                           |
  |                                                                   |
  |  <PURPOSE> A server is requesting a packet from us in DMS_PAK()   |
  |            format.  The parameter packet will contain the OID and |
  |            data base path to find on this server and send back.   |
  |                                                                   |
  |            The packet format is:                                  |
  |                                                                   |
  |            +-------+--------+---------------+---------------+     |
  |            |  OID  | Flags  | DMS_PACK_PATH | DMS_PACK_DATA |     |
  |            |8bytes | 2bytes | String        | String        |     |
  |            +-------+--------+---------------+---------------+     |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   t_sndocd (  server, packet, flags, result  )

string             server, packet,       *result;
hword                              flags;



{

   sint        rc           =    0,
               state        =    0;
   hword       control_flag =    0,
               command      =  RECEIVE_SEGMENT;
   boolean     done         =  FALSE;
   OCB       * object       =    NIL;
   OCD       * component    =    NIL;
   string      db_path      = S_NULL,
               component_pak= S_NULL,
               return_packet= S_NULL;






if( s_len( packet ) > 0 )
{

   object = obj_opn( (OID *) s_buffer( packet ), OBJ_UPDATE ); /* TWEEK*/
   if( OCB_POINTER_OK( object ) )
   {

      s_catb(   & component_pak, s_buffer( packet ) + sizeof( OID ),
                                    s_len( packet ) - sizeof( OID ) );
      ocd_unpack( component_pak, & component );
      s_drop(   & component_pak   );

      if( OCD_POINTER_OK( component ) )
      {

         ocd_dbp( object, component, & db_path );
         dms_sdp( object->dmscb, s_buffer( db_path ), s_len( db_path ) );

         while ( ! done )
         {

            rc = ocd_send( object, component, & return_packet,
                                                        & control_flag );
            if( rc == 0 )
            {

               rc = t_send( server, command, flags, state, return_packet );

            }
            else if ( rc == DMS_PACK_FAILED )
            {

               control_flag = DMS_PACK_FAILED;
               break;

            }
            if( control_flag == OCD_SEND_FIRST_AND_LAST ||
                control_flag == OCD_SEND_LAST_OF_MANY   ||
                control_flag == OCD_SEND_SEQUENCE_ERROR ||
                control_flag == DMS_PACK_ERROR          ||
                control_flag == DMS_PACK_FAILED            )
            {

               done = TRUE;

            }
            else
            {

               rc = t_waitc( server, result );
           /*  rc = t_que(   ISCF_RECEIVE, server, result ); */

            }
         }
         s_drop(  & db_path       );
         s_drop(  & return_packet );
         ocd_drp( & component     );

      }
      else
      {

         rc = INVALID_OCD_PACKET;

      }
   }
   else
   {

      rc = OID_NOT_IN_SOIDI;

   }
   rc = obj_cls( object );

}
else
{

   rc = INVALID_DATA_PACKET;

}

if( rc != 0 )
{

   t_csend( server, rc, S_NULL );

}

   return(rc);

}
