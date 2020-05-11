/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    rm_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 17, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  OCB of mail object or    |
  |                                          object to derive mail    |
  |                                          object from.             |
  |                                                                   |
  |  <PARAM>   modifier    string     INPUT                           |
  |  <PARAM>   level       hword      INPUT                           |
  |  <PARAM>   params      byte *     OUTPUT                          |
  |                                                                   |
  |  <PURPOSE> Remove Mail.  Will take either Object input parameter  |
  |            as the OCB of a piece of mail to remove,  Modifier as  |
  |            the Item ID to remove or if Level is OID_PROVIDED      |
  |            then Params can point to an OID of mail to remove.  If |
  |            none of the above are passed and level is INTERACTIVE  |
  |            then a Form is given to ask for Mail Item ID's to      |
  |            remove.                                                |
  |                                                                   |
  |  <OUTPUT>  sint                          Error code.              |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"


sint     rm_mail(  object, modifier, level, params  )

OCB               *object;
string                     modifier;
hword                                level;
byte                                       *params;

{

   sint        rc               =      0,
               form_return      =      0,
               amount_to_remove =      0;
   string      state            = S_NULL,
               user_data        = S_NULL,
               result           = S_NULL,
               ITEMS            = S_NULL;
   OCB       * list_object      =    NIL;






if(  ( level != OBJ_INTERACTIVE  )  &&  ( level != OBJ_OID_PROVIDED )
             &&  ( object == NIL )  &&  ( s_len( modifier ) < 1 )  )
{

   level = OBJ_INTERACTIVE;

}
else if ( s_len( modifier ) > 0 )
{

   level = OBJ_MODIFIER_PROVIDED;

}
else if ( OCB_POINTER_OK( object ) )
{

   level = OBJ_OCB_PROVIDED;

}

switch ( level )
{

   case  OBJ_INTERACTIVE :

      if( TM_master->usrtype == USER_SERVER )
      {

         s_init( & state, "5260" );

         form_return = t_form( state, NIL, NIL );

         if( form_return == FORM_NOT_FORMED )
         {

            rc = TM_FORM_NOT_FORMED ;
            break ;

         }
         if( form_return == FORM_QUIT )
         {

            rc = USER_REQUESTED_QUIT ;
            break ;

         }
         else if( form_return == FORM_ABANDON )
         {

            rc = USER_REQUESTED_ABANDON ;
            break ;

         }
         s_init( & ITEMS, "ITEMS" );
         s_copy( & user_data, var_get( ITEMS ), 0, 0 );
         modifier = user_data;
         s_drop( & state );
         s_drop( & ITEMS );

      }
      else
      {

         rc = NOT_A_REAL_USER ;
         break;

      }

   case  OBJ_MODIFIER_PROVIDED :

            list_object = obj_all( LIST_OBJ, LOCAL_OBJECT, NIL );
            if( OCB_POINTER_OK( list_object ) )
            {

               rc = obj_item( modifier, OBJ_NEED_MANY_OID, NIL,
                            &list_object, NIL, NIL, ANY_OBJ );
               if( rc == 0 )
               {

                  obj_find( list_object, "LIST.SIZE", & amount_to_remove );

               }
            }
            else
            {

               rc = CANNOT_CREATE_OBJECT ;

            }
            break ;

   case  OBJ_OID_PROVIDED :

            amount_to_remove = 1;

            object = obj_opn( (OID *) params, OBJ_UPDATE );
            rc = rm_mail( object, S_NULL, OBJ_OCB_PROVIDED, NIL );

            break;

   case  OBJ_OCB_PROVIDED :

            amount_to_remove = 1;

            if( TM_master->usrtype != MASTER_SERVER )
            {


               rc = t_remote( TM_master->MS, MAIL_OBJ, REMOVE_ACT,
                              OBJ_OCB_PROVIDED, object, S_NULL,
                              & result                             );

            }
            else
            {


            }
            break;

   default :

            break;

}



   return(rc);

}
