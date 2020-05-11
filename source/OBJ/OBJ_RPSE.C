/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_rpseg()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 14, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  Object to put data.      |
  |  <PARAM>   component   OCD *      INPUT  Where in object is the   |
  |                                           data to go?             |
  |  <PARAM>   segment     string     INPUT  Where to get segment data|
  |                                                                   |
  |  <PURPOSE> Update an entire segment of an object by providing the |
  |            full segment data as a string.                         |
  |                                                                   |
  |            Do not maintain cache with obj get/replace.            |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_sdp(), dms_rep(), dms_qdl()                        |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   obj_rpseg(  object, component, segment  )

OCB              * object;
OCD              * component;
string             segment;

{

   string     db_path      = S_NULL ;

   byte      *dms_segment  =   NULL ,
             *seg_holder   =   NULL ,
             *ret_code     =   NULL ;

   ODT       *odt_entry    =   NULL ;

   sint       rc           =   0    ,
              spare        =   0    ,
              seglen       =   0    ;

   boolean    got_from_DMS =    TRUE;






if( ! OCB_POINTER_OK( object ) )
{

    rc = INVALID_OCB;

}
else
{

   odt_entry = odt_find( object, component );

   if ( ! ODT_POINTER_OK( odt_entry ) )
   {

      rc = OCD_NOT_IN_ODT;

   }
   else
   {

      if( s_len( segment ) < 1 )
      {

         rc = INVALID_DATA_POINTER ;

      }
   }
}

if( rc == 0 )
{

   ocd_dbp( object, component, & db_path );


   rc = dms_sdp( object->dmscb, s_buffer( db_path ), s_len( db_path ) );
   rc = dms_sd(  object->dmscb, s_buffer( segment ), s_len( segment ) );
   rc = dms_rpl( object->dmscb );
   if( rc == DMS_REPLACE_DATA_STORED )
   {
      rc = 0;
   }

   s_drop( & db_path );

}


   return(rc);

}
