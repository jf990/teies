/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_sync()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 20, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> To close all open object in order to synchronize the   |
  |            system and make sure unneeded OCB's are not hanging    |
  |            around.  This routine will only *flush* system objects.|
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   ocb_inta (), obj_cls ()                                |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"





sint   obj_sync(  )


{


   OCB        * object       =   NIL,
              * next_object  =   NIL;
   boolean      ok_to_close  = FALSE;




   object = ocb_inta( (OID *) -1, (hword) 0 );

   while( OCB_POINTER_OK( object ) )
   {


      next_object = object->prevocb;
      ok_to_close = FALSE;

      if(   object->type >= START_SYSTEM_INDEX_RANGE
        ||  object->type <=   END_SYSTEM_INDEX_RANGE )
      {

         /* For now ignore OBJ_SYNC mode and close all objects. */
         if( object->type == SOIDI_OBJ   ||
             object->type ==   PSI_OBJ      )
         {

            obj_set_mode( object, OBJ_FLUSH );

         }
         else
         {

            obj_reset_mode( object, OBJ_FLUSH );

         }
         obj_reset_mode( object, OBJ_SYNC_PENDING );
         ok_to_close = TRUE;

      }

      if( ok_to_close )
      {

         object->open_count = 0;
         obj_cls( object );

      }

      object = next_object;

   }



   return(0);
}
