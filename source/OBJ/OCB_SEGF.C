/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocb_segflush                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 10, 1990                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT  Object to flush segment. |
  |  <PARAM>   request_flag byte      INPUT  How to handle the flush. |
  |             OCB_CACHE_CLEAR              Write to dms and also    |
  |                                           clear out all segment   |
  |                                           cache flags/memory.     |
  |             OCB_CACHE_FLUSH              Only write out the segmnt|
  |                                           to the DMS but leave it |
  |                                           in the OCB.             |
  |  <PURPOSE> Write to the DMS any pending segment data that may be  |
  |            held in the object's segment cache.  Only write it out |
  |            if it has been changed by checking the SYNC mode.      |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_sd(), dms_sdp(), dms_rpl(), os_memd().             |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   ocb_segflush(   OCB * object, byte request_flag  )


{

   sint        rc         =      0;





if( OCB_POINTER_OK( object ) )
{

   if( (obj_test_mode( object, OBJ_CACHE_PENDING ))
    && (object->last_seg != NIL)  &&  (s_len( object->lastdbp ) > 0) )
   {

      if( obj_test_mode( object, OBJ_CACHE_UPDATE ) )
      {

         rc = dms_sdp( object->dmscb, s_buffer( object->lastdbp ),
                                         s_len( object->lastdbp ) );
         if( rc == DMS_SDP_OK )
         {

            rc = dms_sd(  object->dmscb, object->last_seg,
                                                  object->last_seg_len );
            if( rc == DMS_SD_OK )
            {

               rc = dms_rpl( object->dmscb );
               if( rc == DMS_REPLACE_DATA_STORED )
               {

                  rc = 0;

               }
               else
               {

                  rc = DATA_NOT_REPLACED;

               }
            }
         }
      }
      switch( request_flag )
      {

         case OCB_CACHE_CLEAR :

            s_drop( & ( object->lastdbp ) );
            os_memd(    object->last_seg  );
            object->last_seg     = NIL;
            object->last_seg_len =   0;
            obj_reset_mode( object, OBJ_CACHE_PENDING );
            obj_reset_mode( object, OBJ_CACHE_UPDATE  );

         case OCB_CACHE_FLUSH :

            break;

         default              :

            rc = OBJ_FUNCTION_NOT_SUPPORTED;
            break;

      }
   }
   else
   {

      obj_reset_mode( object, OBJ_CACHE_PENDING );
      obj_reset_mode( object, OBJ_CACHE_UPDATE  );

   }
}
else
{

   rc = INVALID_OCB;

}



   return(rc);

}
