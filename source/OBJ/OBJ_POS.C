/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_pos()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 11, 1990                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT                           |
  |  <PARAM>   component   OCD *      INPUT                           |
  |                                                                   |
  |  <PURPOSE> Return the relative position of a segment from the     |
  |            parent segment at that level.  Good to determine where |
  |            a given index is relative to the start of the index.   |
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




sint   obj_pos( object, component )

OCB           * object;
OCD           * component;

{

   sint        rc         =      0;
   string      db_path    = S_NULL;





if( OCB_POINTER_OK( object ) )
{

   if( OCD_POINTER_OK( component ) )
   {

      rc = ocd_dbp( object, component, & db_path );
      if( rc == 0 )
      {

         rc = dms_sdp( object->dmscb, s_buffer( db_path ), s_len( db_path ) );
         if( rc == DMS_SDP_OK )
         {

            rc = dms_pos( object->dmscb );

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

   }
   else
   {


   }
}
else
{

   rc = 0;

}




   return(rc);

}
