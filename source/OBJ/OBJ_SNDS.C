/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_sndseg()                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 23, 1989                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT  Object                   |
  |  <PARAM>   component   OCD *      INPUT  Component containing     |
  |                                           segment.                |
  |  <PARAM>   server      string     INPUT  Who sent the packet.     |
  |  <PURPOSE> Send an entire segment of an object described by the   |
  |            component to another server.  The packet format is:    |
  |                                                                   |
  |                     +=====+=========+=========+                   |
  |                     |     |component|segment  |                   |
  |                     | OID | packet  | string  |                   |
  |                     |<-8->|<-s_len->|<-s_len->|                   |
  |                     +=====+=========+=========+                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"



sint   obj_sndseg( object, component, server )

OCB              * object;
OCD              * component;
string             server;

{

   sint        rc               =      0,
               packet_length    =      0;
   hword       state            =      0,
               flags            =      0;
   string      packet           = S_NULL,
               component_packet = S_NULL,
               segment          = S_NULL;






if( OCB_POINTER_OK( object ) )
{

   if( OCD_POINTER_OK( component ) )
   {

      if( s_len( server ) > 0 )
      {

         rc = obj_gtseg( object, component, & segment );
         if( rc == 0 )
         {

            rc = ocd_pack( & component_packet, component );
            packet_length = sizeof( OID ) + s_len( component_packet ) +
                                            s_len( segment )          ;
            s_alloc( & packet, packet_length );
            os_memc( &(object->oid), s_buffer( packet ), sizeof( OID ) );
            os_memc( component_packet, s_buffer( packet) + sizeof( OID ),
                                s_len( component_packet ) + S_OVERHEAD );
            os_memc( segment, s_buffer( packet ) + sizeof( OID ) +
                                 s_len( component_packet ) + S_OVERHEAD,
                                          s_len( segment ) + S_OVERHEAD );
            s_smp_ul( packet ) = packet_length;


            rc = t_send( server, SEND_SINGLE_SEGMENT, flags, state, packet );

#if 0
/* For now, do not return a confirmation. */
            if( rc == 0 )
            {

               rc = t_waitc( server, NIL );

            }

#endif

            s_drop( & packet           );
            s_drop( & component_packet );
            s_drop( & segment          );

         }
      }
      else
      {

         rc = NO_SERVER_SPECIFIED;

      }
   }
   else
   {

      rc = OCD_POINTER_INVALID;

   }
}
else
{

   rc = INVALID_OCB;

}

   return(rc);

}
