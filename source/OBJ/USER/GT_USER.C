/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <NAME>    gt_user                                                |
  |                                                                   |
  |  <AUTHOR>  John L. Foster, Rana Ajaz                              |
  |                                                                   |
  |  <DATE>    November 14, 1988                                      |
  |                                                                   |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   user        OCB *      INPUT  User OCB to get.         |
  |  <PARAM>   modifier    string     INPUT  not used                 |
  |  <PARAM>   level       hword      INPUT  not used                 |
  |  <PARAM>   params      byte *     INPUT  not used                 |
  |                                                                   |
  |  <PURPOSE> Display full information about a particular user.      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
  |                                                                   |
  |  <MODIFICATIONS> Fixed the function keys logic : Oct. 17, 1989    |
  |                                                                   |
  |  <END>                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_prim.h"
#include "form.h"
#include "vars.h"


sint     gt_user( user, modifier, level, params )

OCB              *user;
string            modifier;
hword             level;
byte             *params;

{

   string   form         = S_NULL,
            nickname     = S_NULL,
            full_name    = S_NULL,
            NAME         = S_NULL;

   sint     rc           =      0,
            end_function =      0;

   boolean  done_with_get=  FALSE;

   OCD      *ocd         =  NIL;





   if( OCB_POINTER_OK( user ) )
   {

      if( user->type == USER_OBJ )
      {

         s_init( & form,    "1111"    );
         s_init( & NAME,    "NAME"    );

         while( ! done_with_get )
         {
            rc = 0;


            gt_name( user, S_NULL, FULL_TEIES_NAME, & full_name );

            var_set( NAME, full_name );


            end_function = t_form( form, S_NULL, user );

            if( end_function == USER_REQUESTED_REPLY )
            {

               ocd_new( & ocd, "NAME.NICKNAME"     );
               rc = obj_get( user, ocd, & nickname );

               if( rc == 0 )
               {
                  rc = cr_mail( NIL, nickname,
                                CREATE_MAIL_SEND_TO_MODIFIER, NIL );

                  if( rc ==  USER_REQUESTED_ABANDON )
                  {

                      done_with_get = TRUE;

                  }

               }

               ocd_drp( & ocd );

            }
            else
            {

               rc = end_function;
               done_with_get = TRUE;

            }

         }
         f_close(  form, NO_FLAGS );
         s_drop( & form           );

         var_clr( NAME  );
         s_drop( & NAME );

      }
   }
   else
   {

      rc = INVALID_OCB;

   }



   return(rc);

}
