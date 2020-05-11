/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_cmod(), obj_change_mode()                          |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 7, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  OCB to alter mode of.    |
  |  <PARAM>   mode        hword      INPUT  Mode bit flags to set to.|
  |                                                                   |
  |  <PURPOSE> Change the mode of an open object by the following     |
  |            rules:                                                 |
  |                                                                   |
  |            If the object has the OBJ_READ flag set and the mode   |
  |            change requests OBJ_UPDATE, then close the object and  |
  |            re-open it in update mode forcing a new  view and al-  |
  |            lowing future updates to occur.                        |
  |                                                                   |
  |            If the object has the OBJ_UPDATE flag set and the mode |
  |            change requests OBJ_READ, close-flush the object to    |
  |            commit prior updates and re-open the object in Read    |
  |            mode.  Future updates to the object will not be allowed|
  |                                                                   |
  |            Any other mode changes have no effect except to alter  |
  |            (or toggle, in case of conflict) the mode flags.       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   obj_cmod(  object, mode  )

OCB *             object;
hword             mode;


{

   sint        rc              =     0;
   hword       mode_set_flag   =     0,
               mode_reset_flag =     0,
               dms_close_mode  =     0,
               dms_open_mode   =     0;
   boolean     change_mode     = FALSE;




if( OCB_POINTER_OK( object ) )
{


   if( (object->mode & OBJ_READ) && (mode & OBJ_UPDATE) )
   {

      mode_set_flag   = OBJ_READ          ;
      mode_reset_flag = OBJ_UPDATE        ;
      dms_close_mode  = DMS_CLOSE_BACKOUT ;
      dms_open_mode   = DMS_OPEN_UPDATE   ;
      change_mode     = TRUE              ;

   }
   else if( (object->mode & OBJ_UPDATE) && (mode & OBJ_READ) )
   {

      mode_set_flag   = OBJ_UPDATE         ;
      mode_reset_flag = OBJ_READ           ;
      dms_close_mode  = DMS_CLOSE_COMMIT   ;
      dms_open_mode   = DMS_OPEN_READ_ONLY ;
      change_mode     = TRUE               ;

   }
   else
   {

      obj_set_mode( object, mode );
      change_mode = FALSE;

   }

   if( change_mode )
   {

      object->mode &= ~ mode_set_flag;

      rc = dms_cls( object->dmscb, dms_close_mode );
      if( rc != DMS_CLOSE_OK )
      {


      }
      else
      {

         rc = 0;

      }
      object->dmscb = dms_opn( object->lfd, object->goid, dms_open_mode );
      if( object->dmscb == NIL )
      {

         rc = DMS_OPEN_ERROR;

      }
      object->mode |= mode_reset_flag;

   }
}
else
{

   rc = INVALID_OCB;

}


   return(rc);

}
