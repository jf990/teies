/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cfr_set                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 26, 1990                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   user_oid    OID *      INPUT  User whose conference    |
  |                                           marker we are to update.|
  |  <PARAM>   membership  OID *      INPUT  Membership list of the   |
  |                                           conference.             |
  |  <PARAM>   time_stamp  datetime * INPUT  Update as of this date & |
  |                                           time.                   |
  |  <PARAM>   cc_seq_num  sint       INPUT  Internal conference com- |
  |                                           ment sequence number.   |
  |  <PARAM>   cc_count    sint       INPUT  Number of waiting com-   |
  |                                           ments.                  |
  |  <PARAM>   conference  OCB *      INPUT  Conference object.       |
  |  <PURPOSE> Update a users conference marker given the above par-  |
  |            ameters.  This sends a packet to the server responsible|
  |            for the conference and updates the membership index.   |
  |            The packet format is:                                  |
  |                                                                   |
  |         +---------+-----------+----------+----------+----------+  |
  | Element>|  OID of | OID of    | Date/Time| Comment  | Comment  |  |
  |         |  User   | Membership| Stamp    | Sequence | Position |  |
  |         |         |           |          | Number   | Counter  |  |
  | Size   >|    8    |     8     |    8     |    4     |    4     |  |
  |         +---------+-----------+----------+----------+----------+  |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "obj_cr.h"




sint   cfr_set(  user_oid, membership, time_stamp, cc_seq_num, cc_count,
                                                      conference_object )

OID            * user_oid,
               * membership;
datetime       * time_stamp;
sint             cc_seq_num,
                 cc_count;
OCB            * conference_object;


{

   sint        rc         =      0;
   string      packet     = S_NULL,
             * result_str =    NIL;
   byte      * packet_ptr =    NIL;





if(  OID_POINTER_OK( user_oid ) &&   OID_POINTER_OK( membership ) &&
   ! OID_IS_NULL(    user_oid ) && ! OID_IS_NULL(    membership )    )
{

   if( TM_master->usrtype == USER_SERVER )
   {


      s_alloc( & packet,    CONFERENCE_MARKER_PACKET_SIZE );
      s_smp_ul(  packet ) = CONFERENCE_MARKER_PACKET_SIZE  ;
      packet_ptr          = s_buffer( packet );

      os_memc( user_oid, packet_ptr,     sizeof( OID ) );
      packet_ptr                      += sizeof( OID );

      os_memc( membership, packet_ptr,   sizeof( OID ) );
      packet_ptr                      += sizeof( OID );

      os_memc( time_stamp, packet_ptr,   sizeof( datetime ) );
      packet_ptr                      += sizeof( datetime );

      os_memc( & cc_seq_num, packet_ptr, 4 );
      packet_ptr                      += 4  ;

      os_memc( & cc_count, packet_ptr,   4 );
      packet_ptr                      += 4  ;


      rc = t_remote( TM_master->MS, CONF_OBJ, JOIN_ACT, (hword) 0,
                                conference_object, packet, result_str  );

      if( rc == NO_ERROR )
      {

         rc = t_waitc( TM_master->MS, result_str );

      }
      s_drop(   result_str );
      s_drop( & packet     );

   }
   else
   {


   }
}
else
{

  rc = INVALID_OID;

}


   return(rc);

}
