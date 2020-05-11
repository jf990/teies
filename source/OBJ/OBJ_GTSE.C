/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_gtseg()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 14, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  Object to get data from. |
  |  <PARAM>   component   OCD *      INPUT  Where in object is the   |
  |                                           data ?                  |
  |  <PARAM>   segment     string *   OUTPUT Where to place result    |
  |                                           segment converted to str|
  |                                                                   |
  |  <PURPOSE> Return a pointer to an entire DMS segment as the result|
  |            of a Get operation.                                    |
  |                                                                   |
  |            Maintain OCB segment cache with obj get/replace.       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   dms_sdp(), dms_find(), dms_qdl()                       |
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




sint   obj_gtseg(  object, component, segment  )

OCB              * object;
OCD              * component;
string           * segment;

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

      if( segment == NIL  || segment == (string *) -1 )
      {

         rc = INVALID_DATA_POINTER ;

      }
   }
}

if( rc == 0 )
{

   ocd_dbp( object, component, & db_path );



                                  /*************************************/
                                  /* Check to see if the new DBP is the*/
                                  /* same as the current DBP.  If so,  */
                                  /* then the segment in question is   */
                                  /* (or should be) already in the OCB.*/
                                  /*************************************/

   if ( 0 == s_comp (  object->lastdbp, db_path, 0, 0, 'M', & spare  ) )
   {

      dms_segment = object->last_seg;
      seglen      = object->last_seg_len;

      got_from_DMS = FALSE;

   }
   else
   {


      dms_sdp(  object->dmscb, s_buffer ( db_path ), s_len ( db_path )) ;
      rc = dms_find (  object->dmscb  )  ;
                                  /* Drop old dbp and set new last one */
      s_drop( & (object->lastdbp)                   ) ;
      s_copy( & (object->lastdbp),  db_path,       0, 0 );
      s_copy( & (object->last_ocd), component->name, 0, 0 );
      got_from_DMS = TRUE;

      if( rc != DMS_FIND_NO_DATA_FOUND )
      {

         rc = 0;
         dms_segment = dms_qd  (object->dmscb) ;
         seglen = dms_qdl (object->dmscb) ;

      }
      else
      {

         rc = NO_DATA_FOUND;

      }
   }
                                  /*-----------------------------------*/
                                  /* Check to see if something found.  */
                                  /* If not, return the err message but*/
                                  /* first make sure the DBP is cleared*/
                                  /* so we do not confuse this failure */
                                  /* on the next obj operation.        */
                                  /*-----------------------------------*/

   if (  dms_segment == NULL || seglen == 0  )
   {

      if( rc == 0 ) {    rc = NO_DATA_FOUND;    }
      s_drop( &(object->lastdbp) );

   }
   else
   {

      s_nulout( segment );
      s_catb(   segment, dms_segment, seglen  );

   }
}


   return(rc);

}
