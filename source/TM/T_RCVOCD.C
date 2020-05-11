/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_rcvocd()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    12/26/88                                               |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server      string     INPUT  Requesting server.       |
  |  <PARAM>   packet      string     INPUT  DMS_PAK data.            |
  |  <PARAM>   flags       hword      INOUT  Sequence indicators.     |
  |  <PARAM>   result      string    OUTPUT  Result storage.          |
  |                                                                   |
  |  <PURPOSE> A server is sending us a packet that was packed using  |
  |            dms_pak(), we are to unpack it.  The first 8 bytes of  |
  |            the packets are the object ID (OID) of the object to   |
  |            get the packet.  After that is a string containing the |
  |            data packed with dms_pak() to be used in the call to   |
  |            dms_unpk() to put the data in the object.              |
  |                                                                   |
  |            The flags tell us if more packets are coming (could not|
  |            send all the object data in one packet).  The packet   |
  |            format is:                                             |
  |                                                                   |
  |            +-------+--------+---------------+---------------+     |
  |            |  OID  | Flags  | DMS_PACK_PATH | DMS_PACK_DATA |     |
  |            |8bytes | 2bytes | String        | String        |     |
  |            +-------+--------+---------------+---------------+     |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   s_len(), dms_unpk(), obj_opn(), t_sendc()              |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"

extern void dmsdcb();


sint   t_rcvocd (  server, start_packet, flags, result  )

string             server, start_packet,       *result;
hword                                    flags;



{

   sint        rc              =      0,
               i               =      0,
               state           =      0,
               watchdog        =      0;
   hword       control_flag    =      0,
               len             =      0,
               command         =      0;
   OCB        *object          =    NIL;
   boolean     done            =  FALSE,
               ok              =  FALSE;
   string      dms_data_string = S_NULL,
               dms_path_string = S_NULL,
               packet          = S_NULL,
               next_packet     = S_NULL;
   byte       *junk            =    NIL;



if( s_len( start_packet ) > 0 )
{


   object = obj_opn(  (OID *)  s_buffer( start_packet ), OBJ_UPDATE );


   if( OCB_POINTER_OK( object ) )
   {

      s_copy( & packet, start_packet, 0, 0 );
      while( ! done )
      {

         control_flag = *((hword *)(s_buffer( packet ) + sizeof( OID )));

         dms_path_string = (string) (s_buffer( packet ) + sizeof( OID ) + 2 );

         dms_sdp( object->dmscb, s_buffer( dms_path_string ),
                                 s_len(    dms_path_string ) );
/* dms_dar( object->dmscb, 0, 0, TRUE );  */
         rc = dms_rmv( object->dmscb );
/* dms_dar( object->dmscb, 0, 0, TRUE );  */


         dms_data_string = (string) (s_buffer( dms_path_string ) +
                                            s_allen( dms_path_string ) );



         rc = ocd_recv( object, dms_data_string );
/* dms_dar( object->dmscb, 0, 0, TRUE ); */



         if( control_flag == OCD_SEND_SEQUENCE_ERROR ||
             control_flag == OCD_SEND_FIRST_AND_LAST ||
             control_flag == OCD_SEND_LAST_OF_MANY   ||
             control_flag == DMS_PACK_ERROR            )
         {

            done = TRUE;

         }
         else
         {


            t_send( server, CONFIRMATION, flags, state, S_NULL );

            while(
                  (rc = t_recv( server, & command, & flags, & state, & packet))
                   != 0 )
            {


               if( SERVER_NOT_CONNECTED( rc ) )
               {

                  ok = FALSE;
                  t_cls_t( server, CLOSE_CONNECTION );
                  break;

               }
               os_wint( 1, 1000 );
               if( ++watchdog > RECEIVE_WATCHDOG_LIMIT )
               {

                  if( watchdog < RECEIVE_WATCHDOG_LIMIT  )
                  {

                    except( rc, CONTROL_ERROR, server, S_NULL, S_NULL,
                                                       S_NULL, S_NULL );

                  }
                  ok   = FALSE;
                  done = TRUE;
                  break;

               }

            }
         }
      }

              /*+-----------------------------------------------------+
                | Once we received the updates the object should be   |
                | committed to the data base.                         |
                +-----------------------------------------------------+*/

      obj_set_flag( object, OBJ_FLUSH );
      obj_cls(      object            );
      s_drop(     & packet            );

   }
   else
   {

      rc = OID_NOT_IN_SOIDI;

   }

}
else
{

   rc = INVALID_OCD_PACKET;

}

rc = t_csend( server, rc, S_NULL );

   return(rc);

}
