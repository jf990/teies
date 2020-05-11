/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    gt_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 23, 1988   (from JAU Danmark)                   |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   mail_ocb    OCB *      INPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |                                                                   |
  |  <PURPOSE> Display a piece of mail in full and process the        |
  |            function requests made by the user at this point.      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "exc.h"
#include "isv.h"
#include "obj_prim.h"

#define USER_REQUESTED_DO    USER_REQUESTED_SCAN


sint     gt_mail( ocb, modifier, level, params )

OCB              *ocb;
string            modifier;
hword             level;
byte             *params;

{


   string   form         = S_NULL;
   sint     rc           =      0,
            xc           =      0,
            end_function =      0;
   boolean  done_with_get=  FALSE,
            ok_to_receive=   TRUE;





   if( OCB_POINTER_OK( ocb ) )
   {

      if( ocb->type == MAIL_OBJ )
      {

         if ( level == ISV_NEW )
         {
            s_init( & form,    "1160"    );
         }
         else
         {
            s_init( & form,    "1163"    );
         }

         while( ! done_with_get )
         {

            end_function = t_form( form, S_NULL, ocb );

            if ( end_function == USER_REQUESTED_PROCESS
            ||   end_function == USER_REQUESTED_ENTER
               )
            {

               done_with_get = TRUE;

            }
            else if ( end_function == USER_REQUESTED_REPLY )
            {

               rc = cr_mail( ocb, S_NULL, (hword) 0, NIL );

               if( rc == USER_REQUESTED_ABANDON )
               {

                  done_with_get =             TRUE;
                  ok_to_receive =            FALSE;

               }
            }
            else if ( end_function == USER_REQUESTED_QUIT )
            {

               rc            = USER_REQUESTED_QUIT;
               done_with_get =                TRUE;
               ok_to_receive =               FALSE;

            }
            else if ( end_function == USER_REQUESTED_ABANDON )
            {

               rc            = USER_REQUESTED_ABANDON;
               done_with_get =                   TRUE;
               ok_to_receive =                  FALSE;

            }
            else if ( end_function == USER_REQUESTED_KEEP )
            {

                                          /* KEEP mail in list */
               ok_to_receive = FALSE;
               done_with_get =  TRUE;

            }
            else if ( end_function == USER_REQUESTED_CONFIRM )
            {

               except( FUNCTION_NOT_IMPLEMENTED, USER_WARNING, S_NULL,
                                        S_NULL, S_NULL, S_NULL, S_NULL );

            }
            else if ( end_function == USER_REQUESTED_DO )
            {


               rc = do_act( ocb, S_NULL, 0, NIL );

               if ( rc == USER_REQUESTED_QUIT || rc == USER_REQUESTED_ABANDON )
               {

                  done_with_get =                TRUE;
                  ok_to_receive =               FALSE;

               }
            }
         }

         if( ok_to_receive && ( level == ISV_NEW ) )
         {

            level = LINK_MAIL_KEY_PROVIDED;
            rc = rc_mail( ocb, modifier, level, NIL );

         }
         f_close(  form, NO_FLAGS );
         s_drop( & form           );

      }
   }
   else
   {

      rc = INVALID_OCB;

   }



   return(rc);

}
