/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    md_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 20, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   mail_object OCB *      INPUT  mail item to modify      |
  |  <PARAM>   item_list   string     INPUT  list of mail items to    |
  |                                           modify.                 |
  |  <PARAM>   level       hword      INPUT                           |
  |  <PARAM>   result      OCB **     OUTPUT Where to put result.     |
  |                                                                   |
  |  <PURPOSE> Modify Mail.                                           |
  |                                                                   |
  |            If mail_object is a mail OCB, that is the object to    |
  |            modify.                                                |
  |                                                                   |
  |            Otherwise, if item_list is not S_NULL, then that is    |
  |            a list of Mail Objects to be verified and modified.    |
  |                                                                   |
  |            If neither are provided, a Form is displayed to ask    |
  |            for a list of mail items.                              |
  |                                                                   |
  |            If we are the master server, no forms are displayed.   |
  |            Therefore, either mail_object or item_list must be     |
  |            provided or nothing will be done.                      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   obj_copy()                                             |
  |  <FORMS>                                                          |
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
#include "vars.h"
#include "form.h"
#include "exc.h"
#include "isv.h"
#include "obj_prim.h"
#include "activity.h"




sint     md_mail( mail_object, item_list, level, result_ocb )

OCB             * mail_object;
string            item_list;
hword             level;
OCB            ** result_ocb;



{

string state          = S_NULL,
       TOPIC_KEYS     = S_NULL,
       SIGNATURE      = S_NULL,
       REPLY_ID       = S_NULL,
       ADD_ACT        = S_NULL,
       DRAFT          = S_NULL,
       SAVE_DRAFT     = S_NULL,
       OPTIONS        = S_NULL,
       OK_TO_SEND     = S_NULL,
       GO_TO_SP       = S_NULL,
       SENDTO1        = S_NULL,
       SENDTO2        = S_NULL,
       TITLE          = S_NULL,
       ACT_CONFIRM    = S_NULL,
       ITEMS          = S_NULL,
       holder         = S_NULL,
       modifier_name  = S_NULL,
       item_prefix    = S_NULL,
       who_to_send_to = S_NULL,
       isv_string     = S_NULL,
       message        = S_NULL,
       word           = S_NULL,
       scratchpad     = S_NULL,
       comma          = S_NULL,
       paren          = S_NULL,
       blank          = S_NULL,
       send_to        = S_NULL,
       result_str     = S_NULL,
       field          = S_NULL,
       field_no       = S_NULL,
       field_yes      = S_NULL,
       attach_id      = S_NULL,
       YorN           = S_NULL,
       number_not_verified = S_NULL,
       association    = S_NULL,
       bad_items      = S_NULL,
       packet         = S_NULL;
OCB   *mail_ocb       =    NIL,
      *reply_ocb      =    NIL,
      *mail_list      =    NIL,
      *act_ocb        =    NIL,
      *scratchpad_object = NIL;
OCD   *ocd            =    NIL,
      *list_component =    NIL,
      *mail_ocd       =    NIL;
sint   rc             =      0,
       end_function   =      0,
       position       =      0;

boolean uploaded         = FALSE,
        edited           = FALSE,
        spad_copied      = FALSE,
        response         = FALSE,
        activity_attach  = FALSE,
        show_association = FALSE,
        list_of_items    = FALSE,
        done             = FALSE,
        linked           = FALSE;

byte  * act_params     =     NIL,
        reply          =   FALSE;
hword   close_flags    =       0,
        level_act      = ACT_LINK_ACT,
        activity_isv   =       0;
real    x              =     0.0,
        y              =     0.0;
OID     author_oid              ,
        message_oid             ,
        activity_oid            ;






   s_init(   & ITEMS,     "ITEMS"         );
   s_init(   & SIGNATURE, "SIGNATURE"     );
   s_init(   & REPLY_ID,  "REPLY_ID"      );
   s_init(   & ADD_ACT,   "ADD_ACTIVITY"  );
   s_init(   & DRAFT,     "DRAFT_SP"      );
   s_init(   & GO_TO_SP,  "GO_TO_SP"      );
   s_init(   & OK_TO_SEND,"OK_TO_SEND"    );
   s_init(   & SAVE_DRAFT,"SAVE_DRAFT"    );
   s_init(   & OPTIONS,   "OTHER_OPTIONS" );
   s_init(   & SENDTO1,   "SEND1"         );
   s_init(   & SENDTO2,   "SEND2"         );
   s_init(   & TITLE,     "TITLE"         );
   s_init(   & ACT_CONFIRM, "ACT_CONFIRM" );
   s_init(   & state,     "4060"          );
   s_init(   & comma,     ","             );
   s_init(   & paren,     "()"            );
   s_init(   & blank,     " "             );
   s_init(   & scratchpad, "A * TEIES $SP");

   isv_get(  & field_no ,  ISV_NO,   1    ); /* DO NOT drop the yes or */
   isv_get(  & field_yes,  ISV_YES,  1    ); /* no ISV strings!        */
   s_init(   & YorN,       "yes or no"    ); /* Should come from ISV ! */

   var_set(     OK_TO_SEND, field_no    );
   var_set(     GO_TO_SP,   field_yes   );



if( OCB_POINTER_OK( mail_object )  && mail_object->type == MAIL_OBJ )
{

   list_of_items = FALSE;

}
else
{

   rc = cr_list( NIL, S_NULL, (hword) 0, & mail_list );

   while( ! done )
   {

      if( s_len( item_list ) > 0 && level != OBJ_INTERACTIVE )
      {

         list_of_items = TRUE;
         level         = OBJ_MODIFIER_PROVIDED;

      }
      else
      {

         end_function  = t_form( state, S_NULL, NIL );
         item_list     = (string) var_get( ITEMS );
         list_of_items = TRUE;
         level         = OBJ_INTERACTIVE;

      }


      rc = obj_item( item_list, OBJ_NEED_MANY_OID, NIL, & mail_list,
                                               S_NULL, NIL, ANY_OBJ );

      if( rc != 0 )
      {

         if( level != OBJ_INTERACTIVE )
         {

            break;

         }
         else
         {


            except( MAIL_ITEMS_DO_NOT_EXIST, USER_ERROR, bad_items, holder,
                                                S_NULL, S_NULL, S_NULL );
            s_nulout( & item_list );

         }
      }
      else
      {

         done = TRUE ;

      }
   }
}

if( s_len( TM_master->ucb->name ) > 0 )
{

   s_copy( & modifier_name, TM_master->ucb->name, 0, 0 );

}
else
{

   s_init( & modifier_name, "(TEIES Phantom User)" );

}

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  We will loop and present the form until the following occur:   |
      |                                                                 |
      |  1 ) User says OK TO MODIFY                and                  |
      |  2 ) User does not request to Quit         and                  |
      |  3 ) There are names on the To: line       and                  |
      |  4 ) There are no errors in the send process (lk_mail()) .      |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

s_init(  & state,  "4061" );

if( list_of_items )
{

   ocd_new( & list_component, "LIST.LIST.OID" );
   list_component->bias = 1;

}

done = FALSE ;

while ( ! done )
{

   /* Get next message OID to edit and open it */

   if( list_of_items )
   {

      rc = obj_get( mail_list, list_component, & message_oid );
      if( rc != 0 )
      {

         done = TRUE ;
         break;

      }
      else if ( OID_IS_NULL( & message_oid ) )
      {

         s_init( & (list_component->name), "LIST.LIST.ENTRY" );
         list_component->bias = 0;
         obj_get( mail_list, list_component, & message );

         s_init( & (list_component->name), "LIST.LIST.OID" );
         list_component->bias = 1;

      }
      else
      {

         mail_object = obj_opn( & message_oid, OBJ_UPDATE | OBJ_REMOTE );
         if( ! OCB_POINTER_OK( mail_object ) || mail_object->type != MAIL_OBJ )
         {

            continue;

         }
      }
   }
   else
   {

      done = TRUE;

   }

   if( OCB_POINTER_OK( mail_object ) )
   {

      ocd_new( & mail_ocd,     "MESSAGE.TO"              );
      obj_get(   mail_object, mail_ocd, & who_to_send_to );
      var_set( SENDTO1, who_to_send_to );

      obj_find( mail_object, "MESSAGE.ACTIVITY", & activity_oid );
      if( ! OID_IS_NULL( & activity_oid ) )
      {

         activity_attach = TRUE;
         s_init( & message,        "("          );

         obj_find( mail_ocb, "MESSAGE.ACT_TYPE", & isv_string );

         s_cat(   & message,    message, isv_string, 0, 0 );
         isv_get( & isv_string, ISV_ACTIVITY,           0 );
         s_cat(   & message,    message, blank,      0, 0 );
         s_cat(   & message,    message, isv_string, 0, 0 );
         isv_get( & isv_string, ISV_ATTACHED,           0 );
         s_cat(   & message,    message, blank,      0, 0 );
         s_cat(   & message,    message, isv_string, 0, 0 );
         s_cat(   & message,    message, paren,      2, 1 );


         f_setfld( state, ADD_ACT,     field_no );
         f_setfld( state, GO_TO_SP,    field_no );
         f_setfld( state, ACT_CONFIRM, message  );
         var_set(  GO_TO_SP,           field_no );
         var_set(  ACT_CONFIRM,        message  );

         s_drop( & message     );

      }

      while( ! linked )
      {

         end_function = t_form( state,  NIL, mail_object );

         if( end_function != TM_FORM_NOT_FORMED
         &&  end_function != USER_REQUESTED_ABANDON
         &&  end_function != USER_REQUESTED_QUIT         )
         {

            if( (response = t_valyn( (string) var_get( ADD_ACT ))) == TRUE )
            {


               rc = md_act( S_NULL );
               if( rc == 0 )
               {

                  f_setfld( state, ADD_ACT, field_no );

               }
               else if ( rc == USER_REQUESTED_ABANDON )
               {
                  break;
               }
               else
               {

                  f_setfld( state, ADD_ACT, field_no );
                  continue;

               }
            }
            else if( response != FALSE )
            {

               f_curpos( state, NO_FLAGS, ADD_ACT, x, y );
               except( INPUT_EXCEPTION, USER_ERROR, (string) var_get( ADD_ACT ),
                                          YorN, S_NULL, S_NULL, S_NULL    );
               continue;

            }
            if( s_type( (string) var_get( DRAFT ), 0, 0, 'N' ) == 0 )
            {

               s_init( & scratchpad, "A * TEIES $SP" );
               s_cat(  & scratchpad, scratchpad, (string) var_get(DRAFT), 0, 0);

            }

    /*******************************************************************\
    |We have figured out which scratchpad to use so now call the scratch|
    |pad for entry of the message text.  If the user ends the editing   |
    |normally we go on to transfer the text into the mail object.  Then |
    |we can post the final screen to fill in the envelope contents.     |
    \*******************************************************************/

            if( t_valyn( (string) var_get( GO_TO_SP ) ) )
            {


               if( ! OCB_POINTER_OK( scratchpad_object ) )
               {

                  scratchpad_object = obj_opn( (OID *) scratchpad,
                                                     OS_FILE | OBJ_UPDATE );
                  if( ! OCB_POINTER_OK( scratchpad_object ) )
                  {

                     except( SCRATCHPAD_CANNOT_EDIT_FILE, CONTROL_ERROR,
                            scratchpad, S_NULL, S_NULL, S_NULL, S_NULL );

                  }
               }

               if( ( ! edited ) && ( ! spad_copied ) )
               {

                  ocd_new( & mail_ocd,  "MESSAGE.CONTENTS" );
                  rc = obj_copy( scratchpad_object, NIL,
                                 mail_object,       mail_ocd );
                  spad_copied = TRUE;

               }

               end_function = t_spad( scratchpad, & scratchpad );
               edited = TRUE;

            }


            if( t_valyn( (string) var_get( OPTIONS ) ) )
            {

               except( FEATURE_NOT_AVAILABLE, USER_ERROR, S_NULL, S_NULL,
                                                  S_NULL, S_NULL, S_NULL );
               f_setfld( state, OPTIONS, field_no       );
               f_curpos( state, NO_FLAGS, OPTIONS, x, y );
               continue;

            }

    /*******************************************************************\
    |If the user toggels OK_TO_SEND or our logic decided to set it, try |
    |to do any local validation and then send the Mail to the Master Ser|
    |for final validation.  If a problem occurs we put the form back up |
    |for the user to correct the trouble.                               |
    |                                                                   |
    |If it is not ok to send, then just keep giving the user back the   |
    |same screen and process whatever haas been requested.              |
    \*******************************************************************/


            s_cat( & who_to_send_to, (string) var_get( SENDTO1 ),
                                     (string) var_get( SENDTO2 ), 0, 0 );
            s_upper(   who_to_send_to,                            0, 0 );
            ocd_new( & mail_ocd,           "MESSAGE.TO"                );
            obj_rep( mail_object, mail_ocd, who_to_send_to, SEG_UPDATE );

            if(
                ( response = t_valyn( (string) var_get( OK_TO_SEND ) ) )
            == TRUE )
            {


               if( s_len( who_to_send_to ) > 0 )
               {

                 obj_update( mail_object, "MESSAGE.MODIFIER", modifier_name );

                 if( edited )
                 {

                    if( OCB_POINTER_OK( scratchpad_object ) )
                    {

                       ocd_new(    & mail_ocd, "MESSAGE.CONTENTS" );
                       rc = obj_copy( mail_object, mail_ocd,
                                      scratchpad_object, NIL      );

                    }
                    edited = FALSE;

                 }
               }
               else
               {

                 except( NO_ADDRESSES_SPECIFIED, USER_ERROR, S_NULL, S_NULL,
                                             S_NULL, S_NULL, S_NULL );
                 f_curpos( state, NO_FLAGS, SENDTO1, x, y );
                 continue;

               }


               rc = 0;
               if ( activity_attach == TRUE )
               {
                    rc = lk_act( act_ocb, packet, level_act, & result_str );
               }
               if ( rc == 0 )
               {
                    rc = lk_mail( mail_object, packet, LINK_MODIFIED_MAIL,
                                                          & result_str );
               }
      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  If the link action returns an exception code, determine the    |
      |  method to handle the problem here.  If it is recoverable then  |
      |  repost the form, wait for user correction, and retry the       |
      |  link action.  Continue until link says no errors or there is   |
      |  an unrecoverable error.                                        |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

               if(( rc - NAME_NOT_FOUND ) < 100 && ( rc - NAME_NOT_FOUND ) > 0)
               {

                  s_sitos( & number_not_verified, rc - NAME_NOT_FOUND );
                  rc       = NAME_NOT_FOUND ;

               }
               switch( rc )
               {

                  case  0                     :
                                           linked     = TRUE;
                                           /* We are done ! */
                                           break;

                  case  NAME_NOT_FOUND        :
                  case  NOT_ALLOWED_TO_SEND_TO:
                                           s_copy( & field, SENDTO1, 0, 0 );
                                           break;

                  case  IMPROPER_KEYWORD      :
                                           s_init( & field, "TOPIC_KEYS"  );
                                           break;

                  default                     :
                                           linked = FALSE;
                                           s_drop( & field );
                                           break;
                                                /* "Fatal Error"      */

               }
               if( rc > 0 )
               {

                  except( rc, USER_ERROR, result_str, number_not_verified,
                                                   S_NULL, S_NULL, S_NULL );
                  s_nulout( & number_not_verified );

                  if( rc > TM_INTERNAL_ERROR && rc < 29000 )
                  {

                     linked = TRUE;
                     break;

                  }
                  else
                  {

                     level = 99;

                  }
               }
               if( s_len( field ) > 0 )
               {

                   f_curpos( state, NO_FLAGS, field, x, y );

               }
            }
            else if ( response == FALSE )
            {

               if( s_len( who_to_send_to ) > 0 )
               {

                  var_set(  OK_TO_SEND, field_yes   );
                  f_setfld( state, OK_TO_SEND, field_yes );
                  f_curpos( state, NO_FLAGS, OK_TO_SEND, x, y );

               }
               else
               {

                  f_curpos( state, NO_FLAGS, SENDTO1, x, y );

               }
               var_set(  GO_TO_SP, field_no    );
               f_setfld( state, GO_TO_SP, field_no );

            }
            else
            {

               f_curpos( state, NO_FLAGS, OK_TO_SEND, x, y );
               except( INPUT_EXCEPTION, USER_ERROR,
                                          (string) var_get( OK_TO_SEND ),
                                          YorN, S_NULL, S_NULL, S_NULL );

            }
         }
         else
         {

            rc = end_function;
            break;

         }
      }

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  At this point 3 things could have happened during the User's   |
      |  interaction process:                                           |
      |  1 ) User said OK TO SEND and the mail was properly linked.     |
      |  2 ) The User requested Quit, therefore linked will be False.   |
      |  3 ) There was a "fatal" error and we cannot continue.          |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

      f_close( state, NO_FLAGS );
      var_clr(  SENDTO1  );
      var_clr(  SENDTO2  );
      var_clr(  TITLE    );
      var_clr(  REPLY_ID );
      var_clr(  ADD_ACT  );
      var_clr(  ACT_CONFIRM );
      obj_cls( scratchpad_object );

      if( linked )
      {

         s_init( & state, "4062"  );

         end_function = t_form( state, NIL, mail_object );

         f_close( state, NO_FLAGS );
         t_delsp( scratchpad );
         close_flags  = OBJ_COMMIT | OBJ_FLUSH ;

      }
      else
      {

         close_flags = OBJ_DELETE;

      }

      obj_cls(      act_ocb       );
      obj_set_flag( mail_object, close_flags );
      obj_cls(      mail_object   );

   }
   else
   {

      rc = MAIL_DOES_NOT_EXIST;
      except( rc, USER_WARNING, bad_items, S_NULL, S_NULL, S_NULL, S_NULL );

   }
}                 /* End while there are still items in list to modify */

s_drop( & holder       );
s_drop( & state        );
s_drop( & field        );
s_drop( & blank        );
s_drop( & paren        );
s_drop( & scratchpad   );
s_drop( & number_not_verified );
s_drop( & OPTIONS      );
s_drop( & TOPIC_KEYS   );
s_drop( & SIGNATURE    );
s_drop( & REPLY_ID     );
s_drop( & ADD_ACT      );
s_drop( & DRAFT        );
s_drop( & GO_TO_SP     );
s_drop( & YorN         );
s_drop( & ACT_CONFIRM  );

obj_set_flag( mail_list, OBJ_DELETE );
obj_cls(      mail_list             );

ocd_drp( & mail_ocd );

   return(rc);

}
