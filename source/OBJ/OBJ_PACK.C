/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_pack                                               |
  |  <AUTHOR>  Tanmay S. Kumar, John L. Foster                        |
  |  <DATE>    June 21, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   senddata string       INPUT    Pointer to send buffer  |
  |  <PARAM>   ocb      struct OCB   IN/OUT   Pointer to the OCB that |
  |                                           contains the pointer to |
  |                                           the ODT                 |
  |  <PARAM>   ocd      struct OCD   INPUT    pointer to OCD          |
  |                                                                   |
  |  <PURPOSE> To pack data for sending to another user ID.           |
  |                                                                   |
  |            Packet format is:                                      |
  |                                                                   |
  |          "Obj_pack_packet_header"                                 |
  |               ( 16 bytes )                                        |
  |                   |                                               |
  |    +--------------+-----------------+                             |
  |    V                                V                             |
  |    +-------+-------+---------+------+---------+------+      +     |
  |    | Object| Object|Number of|Total |SHeader  |DMS   |      |     |
  |    | Type  | ID    |Component|len of|First    |Packet|      |     |
  |    |       |       |Packets  |Data  |DMSPacket|      |      |     |
  |    |<--2-->|<--8-->|<---2--->|<-4-->|<---4--->|<-n-->| . . .|     |
  |    +-------+-------+---------+------+---------+------+      +     |
  |                                 |        |                        |
  |                                 +----+---+                        |
  |                                      |                            |
  |                                  These are String Headers         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   s_first (), s_comp (), s_drop (), s_init (), s_copy () |
  |            s_len(), s_nulout(), s_cat(),   obj_get().             |
  |                                                                   |
  |            marco - ocb_odt(), odt_nxto(), odt_prvo(), odt_name()  |
  |                    odt_lvl().                                     |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"

#define OBJ_PACK_PACKET_HEADER_LENGTH    16
#define MAXIMUM_INTERMEDIATE_DMS_PACKETS 32



sint obj_pack(  final_packet, ocb, ocd  )

string         *final_packet;
OCB            *ocb;
OCD            *ocd;

{

   sint     rc                         =                 0,
            i                          =                 0,
            result_of_dms_pack         = DMS_PACK_NOT_DONE,
            dummy                      =                 0,
            total_dms_packet_length    =                 0,
            dms_packet_counter         =                 0,
            component_offset           =                 0,
            final_length               =                 0,
            data_length                =                 0;
   hword    dms_packet_length[ MAXIMUM_INTERMEDIATE_DMS_PACKETS ],
            what_to_get                =          OCD_ONLY,
            packet_counter             =                 0,
          * str_header                 =               NIL;
   byte   * tmpdata                    =               NIL,
          * dms_packet_array [ MAXIMUM_INTERMEDIATE_DMS_PACKETS ],
          * current_position_in_packet =               NIL;
   string   total_dms_packet           =            S_NULL,
            db_path                    =            S_NULL;






if ( final_packet == NIL )
{

   rc = INVALID_DATA_POINTER;

}
else if (  ! OCB_POINTER_OK( ocb ) )
{

   rc = INVALID_OCB;

}
else if (  ! OCD_POINTER_OK( ocd ) )
{

   rc = OCD_POINTER_INVALID;

}

if ( rc == 0 )
{

   if( obj_test_mode( ocb, OBJ_CACHE_PENDING ) )
   {

      rc = ocb_segflush( ocb, OCB_CACHE_CLEAR );

   }

   os_memn( dms_packet_array, sizeof( byte * ) *
                                     MAXIMUM_INTERMEDIATE_DMS_PACKETS );

   os_memn( dms_packet_length, sizeof( hword ) *
                                     MAXIMUM_INTERMEDIATE_DMS_PACKETS );

   while( obj_next( ocb, ocd, (void **) & tmpdata,
                    component_offset, what_to_get, & dummy ) == 0 )
   {

      rc = ocd_dbp ( ocb, ocd, & db_path );
      if (rc == 0)
      {

         rc = dms_sdp( ocb->dmscb, s_buffer(db_path), s_len(db_path) );

         if (rc == DMS_SDP_OK )
         {

            rc = 0;

            while ( ( result_of_dms_pack == DMS_PACK_NOT_DONE )
             && ( dms_packet_counter < MAXIMUM_INTERMEDIATE_DMS_PACKETS))
            {

               result_of_dms_pack = dms_pak(
                          ocb->dmscb,
                       &( dms_packet_array [ dms_packet_counter ] ),
                       &( dms_packet_length[ dms_packet_counter ] ),
                           NIL                                        );

               if ( result_of_dms_pack == DMS_PACK_ERROR )
               {

                  rc = result_of_dms_pack;
                  break;

               }
               else if ( rc == DMS_PACK_OK )
               {

                  rc = 0;

               }



               total_dms_packet_length +=
                              dms_packet_length[ dms_packet_counter ];
               dms_packet_counter ++ ;


               if( dms_packet_counter > MAXIMUM_INTERMEDIATE_DMS_PACKETS )
               {

                  break;

               }
            }

            if( result_of_dms_pack == DMS_PACK_EMPTY )
            {

               dms_packet_counter -- ;

            }
            else if( result_of_dms_pack == DMS_PACK_ERROR )
            {

               break;

            }
            if( dms_packet_counter > MAXIMUM_INTERMEDIATE_DMS_PACKETS )
            {

               dms_packet_counter = MAXIMUM_INTERMEDIATE_DMS_PACKETS;
               break;

            }
         }
         else
         {


         }
      }
      else
      {

         rc = DMS_PACK_FAILED ;
         break;

      }

      component_offset   = 1;
      result_of_dms_pack = DMS_PACK_NOT_DONE ;

   }           /* While obj_next() returns a component in root segment */

   if( result_of_dms_pack != DMS_PACK_ERROR )
   {

      final_length =  OBJ_PACK_PACKET_HEADER_LENGTH +
                      total_dms_packet_length       +
                     ( S_OVERHEAD * dms_packet_counter );

      s_alloc(    final_packet,    final_length );
      s_smp_ul( * final_packet ) = final_length  ;
      current_position_in_packet = s_buffer( * final_packet )    +
                                   OBJ_PACK_PACKET_HEADER_LENGTH  ;

      for ( i = 0;   i < dms_packet_counter;  i++ )
      {

         if ( dms_packet_array [ i ] != NIL
         &&   dms_packet_length[ i ] >  0   )
         {

            str_header = (hword *) current_position_in_packet;
           *str_header = dms_packet_length[ i ];
            str_header ++ ;
           *str_header = dms_packet_length[ i ];
            current_position_in_packet += S_OVERHEAD ;

            os_memc( dms_packet_array[ i ],
                     current_position_in_packet,
                     dms_packet_length[ i ]                 );
            current_position_in_packet += dms_packet_length[ i ];
            os_memd( dms_packet_array[ i ] );
            dms_packet_array         [ i ] = NIL;
            dms_packet_length        [ i ] =   0;

         }
      }

   /** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
    ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

      packet_counter = dms_packet_counter;

      os_memc( & (ocb->type),    s_buffer( * final_packet ),      2  );
      os_memc( & (ocb->oid),     s_buffer( * final_packet ) +  2, 8  );
      os_memc( & packet_counter, s_buffer( * final_packet ) + 10, 2  );

      packet_counter = s_len( * final_packet ) -
                                           OBJ_PACK_PACKET_HEADER_LENGTH;
      current_position_in_packet = ( (byte *) (* final_packet) ) +
                                           OBJ_PACK_PACKET_HEADER_LENGTH;
      total_dms_packet             = (string) current_position_in_packet;
      s_smp_al( total_dms_packet ) = packet_counter;
      s_smp_ul( total_dms_packet ) = s_smp_al( total_dms_packet ) +
                                                             S_OVERHEAD ;



      if( ( rc == IMPROPER_DIRECTION_IN_ODT )
       || ( rc == OCD_NOT_IN_ODT            ) )
      {

         rc = 0;

      }
   }
   else
   {

      for ( i = 0;   i < dms_packet_counter;  i++ )
      {

         os_memd( dms_packet_array[ i ] )    ;
         dms_packet_array         [ i ] = NIL;
         dms_packet_length        [ i ] =   0;

      }
   }
}

s_drop( & db_path );

   return(rc);
}
