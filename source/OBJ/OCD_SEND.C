/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_send()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    December 15, 1988                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT  Object Control Block of  |
  |                                          object to get components |
  |                                          to be packetized.        |
  |  <PARAM>   component   OCD *      INPUT  Component Descriptor of  |
  |                                          which data to pack.      |
  |  <PARAM>   packet      string *   INOUT  Where to report back the |
  |                                          resulting packet. On call|
  |                                          this contains the con-   |
  |                                          tinuation DBP.           |
  |  <PARAM>   flags       hword  *   INOUT  Status of pack on return.|
  |                                          MUST be initialised to 0.|
  |  <PURPOSE> Get a component out of an object in DMS_PACK() format  |
  |            for eventual copy to another object or transmission to |
  |            another server.  The Object and Component must be de-  |
  |            fined on entry, as well as provision of a valid string |
  |            pointer where the results are to be stored.  Flags are |
  |            set on return and take on the following meanings:      |
  |                                                                   |
  |              OCD_SEND_FIRST_AND_LAST : The entire component has   |
  |                                        been packed into the packet|
  |                                        provided.                  |
  |                                                                   |
  |              OCD_SEND_FIRST_OF_MANY  : The packet contains the    |
  |                                        first packet with more to  |
  |                                        follow.                    |
  |                                                                   |
  |              OCD_SEND_ONE_OF_MANY    : The packet contains an     |
  |                                        intermediate packet with   |
  |                                        more to follow.            |
  |                                                                   |
  |              OCD_SEND_LAST_OF_MANY   : The packet contains the    |
  |                                        final packet and the op-   |
  |                                        eration is complete.       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   ocd_send(  object, component, packet, flags  )

OCB             * object;
OCD             * component;
string          * packet;
hword           * flags;

{

   sint        rc              =      0;
   hword       dms_data_length =      0,
               dms_path_length =      0,
               packet_size     =      0;
   byte       *dms_path        =    NIL,
              *dms_data_packet =    NIL;
   string      holder          = S_NULL;



if( OCB_POINTER_OK( object ) )
{

   if( OCD_POINTER_OK( component ) )
   {

      if( packet != NIL )
      {

         if( flags != NIL )
         {

            dms_path_length = dms_qpl( object->dmscb   );
            dms_path        = os_mema( dms_path_length );
            os_memc( dms_qdp( object->dmscb ), dms_path, dms_path_length );
            rc = dms_pak( object->dmscb, & dms_data_packet,
                                         & dms_data_length, NIL   );

            if( rc == DMS_PACK_ERROR )
            {

               rc = DMS_PACK_FAILED;

            }
            else if( rc == DMS_PACK_NOT_DONE )
            {

               if( * flags == 0 )
               {

                  rc = OCD_SEND_FIRST_OF_MANY;

               }
               else if( * flags == OCD_SEND_FIRST_OF_MANY ||
                        * flags == OCD_SEND_ONE_OF_MANY      )
               {

                  rc = OCD_SEND_ONE_OF_MANY;

               }
               else
               {

                  rc = OCD_SEND_SEQUENCE_ERROR;

               }
            }
            else
            {

               if( * flags == OCD_SEND_FIRST_OF_MANY ||
                   * flags == OCD_SEND_ONE_OF_MANY       )
               {

                  rc = OCD_SEND_LAST_OF_MANY;

               }
               else if ( * flags == 0 )
               {

                  rc = OCD_SEND_FIRST_AND_LAST;

               }
               else
               {

                  rc = OCD_SEND_SEQUENCE_ERROR;

               }
            }

            if ( rc != OCD_SEND_SEQUENCE_ERROR && rc != DMS_PACK_FAILED )
            {

               *flags          = (hword) rc;
               packet_size     = sizeof( OID ) + 2 +
                                 S_OVERHEAD + dms_path_length +
                                 S_OVERHEAD + dms_data_length;
               s_alloc( packet, packet_size );
               s_smp_ul( * packet ) = packet_size;

               os_memc( &(object->oid), s_buffer( * packet ), sizeof( OID ) );
               os_memc(   flags, s_buffer( * packet ) + sizeof( OID ),  2 );
               holder = (string)(s_buffer( * packet ) + sizeof( OID ) + 2 );
               s_smp_ul( holder ) = dms_path_length;
               s_smp_al( holder ) = dms_path_length;
               os_memc( dms_path, s_buffer( holder ),  dms_path_length );
               holder = (string) (s_buffer( holder ) + dms_path_length );
               s_smp_ul( holder ) = dms_data_length;
               s_smp_al( holder ) = dms_data_length;
               os_memc( dms_data_packet, s_buffer( holder ),
                                                       dms_data_length );
               rc = 0;
               os_memd( dms_path        );
               os_memd( dms_data_packet );

            }
         }
         else
         {

            rc = CANNOT_STORE_RESULT_FLAGS;

         }
      }
      else
      {

         rc = CANNOT_STORE_RESULT_PACKET;

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
