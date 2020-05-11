/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocb_setcache                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 10, 1990                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT  Object to flush segment. |
  |  <PARAM>   segment     byte *     INPUT  Data for the segment.    |
  |  <PARAM>   segment_length  sint   INPUT  Length of data.          |
  |  <PURPOSE> Initialize the object cache and set the OCB straight   |
  |            with cache flag, memory management, and DMS path.      |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_sd(), dms_sdp(), dms_rpl(), os_memd(), ocd_dbp().  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   ocb_setcache(   object, segment, segment_length   )

OCB      *             object;
byte     *             segment;
sint                   segment_length;

{

   sint        rc         =      0;





if( OCB_POINTER_OK( object ) )
{

   if( segment_length > 0 )
   {

      if( obj_test_mode( object, OBJ_CACHE_PENDING ) )
      {

         rc = ocb_segflush( object, OCB_CACHE_CLEAR );

      }
      if( rc == 0 )
      {

         object->last_seg     = os_mema( segment_length );
         object->last_seg_len =          segment_length;
         if( segment == NIL )
         {

            os_memn( object->last_seg, segment_length );

         }
         else
         {

            os_memc( segment, object->last_seg, segment_length );

         }
         obj_set_mode( object, OBJ_CACHE_PENDING );

      }
   }
   else
   {

      rc = NO_DATA_PROVIDED;

   }
}
else
{

   rc = INVALID_OCB;

}



   return(rc);

}
