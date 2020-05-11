/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cr_mail                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT  item to reply to.        |
  |  <PARAM>   modifier    string     INPUT  possible reply to.       |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   result      OCB **     OUTPUT Where to put result.     |
  |                                                                   |
  |  <PURPOSE> Create Mail                                            |
  |                                                                   |
  |            If the OCB passed as the first parameter is a valid    |
  |            mail OCD then that is the mail item to reply to.       |
  |                                                                   |
  |            If the modifier contains a string then it is a mail    |
  |            item ID to be parsed and verified as a reply item.     |
  |            Modifier can also contain a list of user names to send |
  |            to either from the command line or computed from another
  |            process.                                               |
  |                                                                   |
  |            If a reply is determined then format the title of the  |
  |            message to be replyed to from the original title.  The |
  |            addresses will automatically be determined by lk_mail()|
  |            but the user can enter additional addresses on the TO  |
  |            line.                                                  |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "isv.h"
#include "obj_prim.h"




sint     cr_mail( ocb, modifier, level, result_ocb )

OCB              *ocb;
string            modifier;
hword             level;
OCB             **result_ocb;



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
       holder         = S_NULL,
       item_prefix    = S_NULL,
       who_to_send_to = S_NULL,
       word           = S_NULL,
       scratchpad     = S_NULL,
       comma          = S_NULL,
       paren          = S_NULL,
       blank          = S_NULL,
       sp_string      = S_NULL,
       result_str     = S_NULL,
       field          = S_NULL,
       field_no       = S_NULL,
       field_yes      = S_NULL,
       attach_id      = S_NULL,
       YorN           = S_NULL,
       number_not_verified = S_NULL,
       association    = S_NULL,
       packet         = S_NULL;
OCB   *mail_ocb       =    NIL,
      *reply_ocb      =    NIL,
      *act_ocb        =    NIL,
      *sp_ocb         =    NIL;
OCD   *ocd            =    NIL,
      *mail_ocd       =    NIL;
sint   rc             =      0,
       end_function   =      0,
       position       =      0;

boolean uploaded         = FALSE,
        edited           = FALSE,
        response         = FALSE,
        activity_attach  = FALSE,
        show_association = FALSE,
        linked           = FALSE;

byte  *act_params     =    NIL,
       reply          =  FALSE;
hword  close_flags    =      0,
       level_act      = ACT_LINK_ACT,
       activity_isv   =      0;
real   x              =    0.0,
       y              =    0.0;
OID    author_oid             ;







mail_ocb = obj_all( MAIL_OBJ, LOCAL_OBJECT, NIL );

if( OCB_POINTER_OK( mail_ocb ) )
{

   if( s_len( TM_master->ucb->name ) > 0 )
   {

      s_copy( & holder, TM_master->ucb->name, 0, 0 );

   }
   else
   {

      s_init( & holder, "(TEIES Phantom User)" );

   }
   obj_update( mail_ocb, "MESSAGE.AUTHOR", holder );
   s_nulout( & holder );

   s_init(   &SIGNATURE, "SIGNATURE"    );
   s_init(   &REPLY_ID,  "REPLY_ID"     );
   s_init(   &ADD_ACT,   "ADD_ACTIVITY" );
   s_init(   &DRAFT,     "DRAFT_SP"     );
   s_init(   &GO_TO_SP,  "GO_TO_SP"     );
   s_init(   &OK_TO_SEND,"OK_TO_SEND"   );
   s_init(   &SAVE_DRAFT,"SAVE_DRAFT"   );
   s_init(   &OPTIONS,   "OTHER_OPTIONS");
   s_init(   &SENDTO1,   "SEND1"        );
   s_init(   &SENDTO2,   "SEND2"        );
   s_init(   &TITLE,     "TITLE"        );
   s_init(   &ACT_CONFIRM, "ACT_CONFIRM");
   s_init(   &state,     "3160"         );
   s_init(   &comma,     ","            );
   s_init(   &paren,     "()"           );
   s_init(   &blank,     " "            );

   isv_get(   & field_no ,  ISV_NO,   1 ); /* DO NOT drop the yes or no*/
   isv_get(   & field_yes,  ISV_YES,  1 ); /* ISV strings!             */
   s_init(    & YorN,       "yes or no" ); /* Should come from ISV !   */

   var_set(     OK_TO_SEND, field_no    );
   var_set(     GO_TO_SP,   field_yes   );


      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  First we must resolve  the reply to mail item if there is one. |
      |  If so, fill in default field values.                           |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   if( OCB_POINTER_OK( ocb ) )
   {

      OID     act_oid         ;
      string  rootid  = S_NULL;

      /*================================================================+
      |  If message.isv_type is not 0 then already has activity attached|
      |  and we cannot allow a reply.                                   |
      +================================================================*/

      rc = obj_find( ocb, "MESSAGE.ACTIVITY", & act_oid );
      if( rc == 0 && ! OID_IS_NULL( (&act_oid) ) )
      {

         rc = CANNOT_REPLY_TO_ACTIVITY;
         obj_find( ocb, "MESSAGE.ID", & rootid );
         except( rc, USER_ERROR, rootid, S_NULL, S_NULL, S_NULL, S_NULL);
         s_drop( & rootid );
         close_flags = OBJ_DELETE;
         reply = FALSE;
         goto CLEANUP;

      }
      reply = TRUE;

   }
   else if ( s_len( modifier ) > 0 )
   {

      OID     * reply_oid  =  NIL;


      if(
          obj_item( modifier, OBJ_LEVEL_ZERO, & reply_oid,
                    NIL, S_NULL, NIL, ANY_OBJ            )
      == 0 )
      {

         ocb = obj_opn( reply_oid, OBJ_READ );
         if( OCB_POINTER_OK( ocb ) )
         {

            reply = TRUE;

         }
         os_memd( reply_oid );

      }
   }

   ocd_new( & mail_ocd, "MESSAGE.REPLY"              );
   obj_rep(   mail_ocb, mail_ocd, & reply, SEG_UPDATE);

   if( reply == TRUE )
   {

      string separator        = S_NULL,
             reply_to         = S_NULL,
             reply_to_spill   = S_NULL,
             nick_name        = S_NULL,
             author_nick_name = S_NULL;
      OCD  * author_ocd       =    NIL;


      s_init( & separator, ":" );
      reply_ocb = ocb;
      obj_update( mail_ocb, "%LINKS.OWNER", & (reply_ocb->oid) );

      obj_find(   reply_ocb, "MESSAGE.ID", & holder );
      s_trim(   & holder, 'B'                       );
      isv_get(  & item_prefix, ISV_MMSG,       1    );
      s_cat(    & holder, blank,       holder, 0, 0 );
      s_cat(    & holder, item_prefix, holder, 0, 0 );
      var_set(    REPLY_ID,  holder                 );
      s_copy(   & attach_id,           holder, 0, 0 );
      s_nulout( & holder                            );

      s_init(   & (mail_ocd->name), "MESSAGE.TITLE" );
      rc = obj_get( reply_ocb, mail_ocd, & holder   );


      rc = obj_rep(   mail_ocb, mail_ocd, holder, SEG_UPDATE );
      s_drop( & holder );

      s_init( & (mail_ocd->name), "MESSAGE.TO" );
      rc = obj_get( ocb, mail_ocd, & reply_to  );


      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |  Now remove the name of the replying author from the original to|
      |  list so we do not send this mail to ourselves.  We can get the |
      |  author nick name locally since Signon updated that object here.|
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

      obj_find( TM_master->ucb->directory, "NAME.NICKNAME", & nick_name);
      position = s_first( nick_name, reply_to, 0, 0 );

      if( position > 0 )
      {

         string   front_portion = S_NULL,
                  back_portion  = S_NULL;


         if ( position != 1 )
         {
            s_copy( & front_portion, reply_to, 0, position - 1 );
         }

         s_copy( & back_portion,  reply_to,
                             position + s_len( nick_name ) + 2,   0    );


         s_cat(   & reply_to, front_portion, back_portion, 0, 0  );
         s_trim(  & reply_to, 'T'      );
         s_ctrim( & reply_to, 'T', ',' );

      }

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |  Next, add the author of the original mail as the first addresse|
      |  on the reply.  We must go to the MS to get the author's Nicknam|
      |  since it may have changed since this mail was sent. Also note  |
      |  if the original author is already in the To list we do not do  |
      |  the add as there will be duplicate names otherwise.            |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

      rc = obj_find( reply_ocb, "%LINKS.AUTHOR", & author_oid );
      if( ! oid_cmp( TM_master->ucb->directory->oid, author_oid ) )
      {


         ocd_new( & author_ocd, "SNI.OBJECT.NICKNAME" );
         oid_key(   author_ocd,  & author_oid );
         rc = t_fndsys(  SNI_OBJ,  author_ocd, & author_nick_name );


         if( rc == 0 )
         {

            position = 0;
            s_upper( reply_to, 0, 0 );
            position = s_first( author_nick_name, reply_to, 0, 0 );
            if (position == 0)
            {

               s_cat( &author_nick_name, author_nick_name, comma, 0, 0 );
               s_cat( & reply_to, author_nick_name, reply_to,     0, 0 );

            }
            else
            {


            }
         }
         else
         {


         }
      }
      else
      {


      }

      rc = obj_update( mail_ocb, "MESSAGE.TO", reply_to );

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |  Handle field wrap if the Send To names cannot fit on one line. |
      |  SENDTO1 and SENDTO2 map to form fields since forms cannot take |
      |  care of the spillover itself.  TWEEK since we actually cannot  |
      |  know the amount of field space for the names so eventually this|
      |  code should come out of here and be integrated into FORMS.     |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

      if ( s_len( reply_to )  >  62 )
      {

         position = s_last( comma, reply_to, 0, 62 );
         s_copy( & reply_to_spill, reply_to, position + 1,
                                          s_len( reply_to ) - position );
         s_copy( & reply_to,  reply_to, 0, position);

      }

      var_set( SENDTO1, reply_to       );
      var_set( SENDTO2, reply_to_spill );

      s_drop (  & reply_to_spill   );
      s_drop (  & author_nick_name );
      s_drop (  & separator        );
      s_drop (  & reply_to         );
      s_drop (  & nick_name        );
      ocd_drp(  & author_ocd       );

   }
   else
   {

      string  send_to = S_NULL ;

          /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
          |  If the MODIFIER passed tells us who the target recipients |
          |  are, then format that in the field.                       |
          \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

      obj_update( mail_ocb, "%LINKS.OWNER", &(TM_master->ucb->user_id) );
      if( level == CREATE_MAIL_SEND_TO_MODIFIER && s_len( modifier ) > 0)
      {

         if ( s_len( modifier )  >  80 )
         {

            position = s_last( comma, modifier, 0, 80 );
            s_copy( & send_to, modifier, position + 1,
                                         s_len( modifier ) - position );
            s_copy( & modifier, modifier, 0, position);
            var_set( SENDTO2, send_to );

         }
         var_set( SENDTO1, modifier );
         s_drop ( & send_to         );

      }
   }
   s_nulout( & holder );

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  We will  loop and present the form until the following occur:  |
      |                                                                 |
      |  1 ) User says OK TO SEND                  and                  |
      |  2 ) User does not request to Quit         and                  |
      |  3 ) There are names on the To: line       and                  |
      |  4 ) There are no errors in the send process (lk_mail()) .      |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


   while( ! linked )
   {

      end_function = t_form( state,  NIL, mail_ocb );

      if( end_function != TM_FORM_NOT_FORMED
       && end_function != USER_REQUESTED_QUIT
       && end_function != USER_REQUESTED_ABANDON    )
      {

       /****************************************************************\
       | If the user changes the reply_to field, then that entry must be|
       | validated and the item there made the reply to item.           |
       \****************************************************************/

         if( 0 == s_comp( attach_id, var_get( REPLY_ID ), 0, 0, 'F',&rc))
         {


         }

         if( (response = t_valyn( (string) var_get( ADD_ACT ))) == TRUE )
         {


            rc = cr_act( mail_ocb, modifier, level, & act_ocb );
            if( rc == 0 )
            {

               string
                         isv_string  = S_NULL,
                         message     = S_NULL;


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

            s_init( &scratchpad, "A * TEIES $SP" );
            s_cat(  &scratchpad, scratchpad, (string) var_get( DRAFT ), 0, 0 );

         }

    /*******************************************************************\
    |We have figured out which scratchpad to use so now call the scratch|
    |pad for entry of the message text.  If the user ends the editing   |
    |normally we go on to transfer the text into the mail object.  Then |
    |we can post the final screen to fill in the envelope contents.     |
    \*******************************************************************/

         if( t_valyn( (string) var_get( GO_TO_SP ) ) )
         {

            end_function = t_spad( scratchpad, & sp_string  );
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
                                  (string) var_get( SENDTO2 ),   0, 0  );
         s_upper(   who_to_send_to,      0, 0                          );
         ocd_new( & mail_ocd,           "MESSAGE.TO"                   );
         obj_rep(   mail_ocb, mail_ocd,  who_to_send_to, SEG_UPDATE    );

         if( ( response = t_valyn( (string) var_get( OK_TO_SEND ) ) ) == TRUE )
         {


            if( s_len( who_to_send_to ) > 0 )
            {

              if( edited )
              {

                 sp_ocb = obj_opn( (OID *) sp_string,
                                                    OBJ_READ | OS_FILE );
                 if( OCB_POINTER_OK( sp_ocb ) )
                 {

                    ocd_new(    & mail_ocd, "MESSAGE.CONTENTS" );
                    rc = ocd_cont( mail_ocb, mail_ocd, sp_ocb   );
                    obj_cls( sp_ocb );

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
                 rc = lk_mail( mail_ocb, packet, level,   & result_str );
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

            if( ( rc - NAME_NOT_FOUND ) < 100 && ( rc - NAME_NOT_FOUND ) > 0 )
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
            (string) var_get( OK_TO_SEND ), YorN, S_NULL, S_NULL, S_NULL);

         }
      }
      else
      {

         rc = end_function ;
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

   if( linked )
   {

      s_init( & state, "3162"  );
      end_function = t_form( state, NIL, mail_ocb );
      f_close( state, NO_FLAGS );
      t_delsp( sp_string );
      close_flags  = OBJ_COMMIT | OBJ_FLUSH ;

   }
   else
   {

      close_flags = OBJ_DELETE;

   }

   if ( activity_attach == TRUE )
   {
        /* the reason for doing this is because the activity object is */
        /* local and we want to put the id in this object after lk_mail*/

        obj_find( mail_ocb, "MESSAGE.ID", & attach_id );
        obj_upd(  act_ocb,  "ACTIVITY.ID",  attach_id );
        s_drop( & attach_id );

        obj_cls( act_ocb );
   }

CLEANUP:


   obj_set_flag( mail_ocb, close_flags );
   obj_cls(      mail_ocb   );

   s_drop( & holder       );
   s_drop( & state        );
   s_drop( & field        );
   s_drop( & blank        );
   s_drop( & paren        );
   s_drop( & comma        );
   s_drop( & scratchpad   );
   s_drop( & sp_string    );
   s_drop( & number_not_verified );


   if( result_ocb != NIL && linked == TRUE )
   {

      * result_ocb = mail_ocb ;

   }
   s_drop(   & OPTIONS    );
   s_drop(   & TOPIC_KEYS );
   s_drop(   & SIGNATURE  );
   s_drop(   & REPLY_ID   );
   s_drop(   & ADD_ACT    );
   s_drop(   & DRAFT      );
   s_drop(   & GO_TO_SP   );
   s_drop(   & YorN       );
   s_drop(  & ACT_CONFIRM );

}
else
{

   rc = CANNOT_CREATE_OBJECT;

}



   return(rc);

}
