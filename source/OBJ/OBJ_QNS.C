/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_qns()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 17, 1989                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT                           |
  |  <PARAM>   component   byte *     INPUT                           |
  |                                                                   |
  |  <PURPOSE> Return the number of segments under a given component. |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   obj_qns( object, component )

OCB           * object;
byte          * component;

{

   sint        rc         =      0;
   string      db_path    = S_NULL;
   OCD       * ocd        =    NIL;




if( OCB_POINTER_OK( object ) )
{

   if( component != NIL )
   {

      ocd_new( & ocd, component );
      rc = ocd_dbp( object, ocd, & db_path );
      if( rc == 0 )
      {

         rc = dms_sdp( object->dmscb, s_buffer( db_path ), s_len( db_path ) );
         if( rc == DMS_SDP_OK )
         {

            rc = dms_qns( object->dmscb );

         }
         else
         {

            rc = 0;

         }
      }
      else
      {


      }
      s_drop(  & db_path );
      ocd_drp( & ocd     );

   }
   else
   {


   }
}
else
{


}




   return(rc);

}
