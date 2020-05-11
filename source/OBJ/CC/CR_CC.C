/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cr_cc                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 21, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT  not used.                |
  |  <PARAM>   modifier    string     INPUT  not used.                |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   params      byte *     INOUT  not used.                |
  |  <PURPOSE> CReate Conference Comment                              |
  |            Verify that we know the proper conference root the new |
  |            comment is to be linked to.  Either the ocb parameter  |
  |            can be object type CR (conference root) or CC (comment)|
  |            If a comment use the owner link to find the CR oid.    |
  |                                                                   |
  |            cr_ocb(owner conference) should be valid, if not then  |
  |            try to figure it out from 'modifier'. If that also     |
  |            fails, put up a form for conference name.              |
  |                                                                   |
  |            If we're replying to a already existing comment then   |
  |            DOING_REPLY bit will be on in 'level' and 'result_ocb' |
  |            will point to pointer to reply cc_ocb.                 |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   lk_cc()                                                |
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





sint     cr_cc(   ocb, modifier, level, result_ocb )

OCB              *ocb;
string            modifier;
hword             level;
OCB             **result_ocb;



{

string form_id         = S_NULL,
       TITLE_LINE      = S_NULL,
       KEYS_LINE       = S_NULL,
       SIGNATURE       = S_NULL,
       REPLY_ID        = S_NULL,
       reply_id        = S_NULL,
       cc_id           = S_NULL,
       id              = S_NULL,
       tmpstr          = S_NULL,
       bad_itm_str     = S_NULL,
       new_reply_id    = S_NULL,
       ADD_ACT         = S_NULL,
       DRAFT           = S_NULL,
       SAVE_DRAFT      = S_NULL,
       OPTIONS         = S_NULL,
       OK_TO_ADD       = S_NULL,
       GO_TO_SP        = S_NULL,
       CONF_ID         = S_NULL,
       conf_id         = S_NULL,
       tmp_str         = S_NULL,
       scratchpad      = S_NULL,
       sp_string       = S_NULL,
       result_str      = S_NULL,
       var_value       = S_NULL,
       field_no        = S_NULL,
       field_yes       = S_NULL;

sint   tform_rc        = 0     ;

OCB   *cc_ocb     =    NULL,
      *cr_ocb     =    NULL,
      *act_ocb    =    NULL,
      *reply_ocb  =    NULL,
      *memb_ocb   =    NULL,
      *sp_ocb     =    NULL;

OCD   *ocd        =    NULL,
      *cc_ocd     =    NULL;

OID    tmp_oid             ,
      *oid_ptr    =  &tmp_oid;

sint    rc        =      0,
        var_yn    =      0,
        seqnum    =      0,
        bad_itm_num =    0,
        junk      =      0;

boolean uploaded           =  FALSE,
        edited             =  FALSE,
        activity_attach    =  FALSE,
        REPLY_OCB_OPENED   =  FALSE,
        ask_for_conference =  FALSE,
        linked             =  FALSE;

byte  *act_params  =    NULL;

hword  close_flags = 0,
       ocb_type    = 0,
       level_act   = ACT_LINK_ACT;

real   x          =    0.0,
       y          =    0.0;







if( OCB_POINTER_OK( ocb ) && (ocb->type == CR_OBJ) )
{

   cr_ocb = ocb;
   memb_ocb = (OCB *) modifier;

   if( (level & DOING_REPLY) && OCB_POINTER_OK(*result_ocb) )
   {

      reply_ocb = *result_ocb;

   }

cc_ocb = obj_all( CC_OBJ, LOCAL_OBJECT, NULL );

if( OCB_POINTER_OK(cc_ocb) )
{

   s_init(   & SIGNATURE,  "SIGNATURE"    );
   s_init(   & REPLY_ID,   "REPLY_ID"     );
   s_init(   & KEYS_LINE,  "KEYS_LINE"    );
   s_init(   & TITLE_LINE, "TITLE_LINE"   );
   s_init(   & ADD_ACT,    "ADD_ACTIVITY" );
   s_init(   & DRAFT,      "DRAFT_SP"     );
   s_init(   & GO_TO_SP,   "GO_TO_SP"     );
   s_init(   & OK_TO_ADD,  "OK_TO_ADD"    );
   s_init(   & SAVE_DRAFT, "SAVE_DRAFT"   );
   s_init(   & OPTIONS,    "OTHER_OPTIONS");
   s_init(   & form_id,    "3150"         );

   isv_get(  & field_no ,  ISV_NO,   1    );
   isv_get(  & field_yes,  ISV_YES,  1    );
   var_set(    OK_TO_ADD,  field_no       );
   var_set(     GO_TO_SP,  field_yes      );

   if( level & DOING_REPLY )
   {

      obj_find( reply_ocb, "COMMENT.ID",    &reply_id);
      var_set( REPLY_ID, reply_id );

      obj_find( reply_ocb, "COMMENT.TITLE", &tmp_str );
      var_set( TITLE_LINE, tmp_str );

#if 0
murray doesn't want keys copied.../sal072589
      obj_find( reply_ocb, "COMMENT.KEYS",  &tmp_str );
      var_set( KEYS_LINE,  tmp_str );
#endif

      obj_update( cc_ocb, "%LINKS.OWNER", &(reply_ocb->oid) );

   }
   else
   {

      obj_update( cc_ocb, "%LINKS.OWNER", &(cr_ocb->oid) );

   }

   obj_update( cc_ocb, "%LINKS.ROOT", &(cr_ocb->oid) );



      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  We will  loop and present the form until the following occur:  |
      |                                                                 |
      |  1 ) User says OK TO SEND                  and                  |
      |  2 ) User does not request to Quit         and                  |
      |  3 ) There are no errors in the send process (lk_cc()).         |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


   while( (rc == NO_ERR) && (! linked) )
   {

      tform_rc = t_form( form_id, NULL, cc_ocb );

      if ( tform_rc == FORM_NOT_FORMED )
      {

         rc = FORM_NOT_FORMED;

      }
      else if ( tform_rc == USER_REQUESTED_ABANDON )
      {

         rc = USER_REQUESTED_ABANDON;

      }
      else if ( tform_rc == USER_REQUESTED_QUIT )
      {

         rc = USER_REQUESTED_QUIT;

      }
      else                       /* Process forms inputs               */
      {

                                  /* Chk if reply_id has changed?      */
         new_reply_id = var_get( REPLY_ID );
         if(s_comp(reply_id, new_reply_id, 0, 0, 'M', &junk) != 0)
         {

/* are we allowing FULL name of the item(C#cc##), then we will have to */
/* check if the reply item belongs to this conf, else obj_item() will  */
/* need to have conf id prefixed to new_reply_id */
            rc = obj_item(new_reply_id, OBJ_NEED_ONE_OID, &oid_ptr, NULL,
                         &bad_itm_str, &bad_itm_num, ocb_type );
            if( rc == NO_ERR )
            {

               reply_ocb = obj_opn( &tmp_oid, OBJ_UPDATE | OBJ_REMOTE );
               if( OCB_POINTER_OK(reply_ocb) )
               {

                  REPLY_OCB_OPENED = TRUE;
                  s_copy( &reply_id, new_reply_id, 0, 0 );

                  obj_find( reply_ocb, "COMMENT.ID",    &reply_id);
                  var_set( REPLY_ID, reply_id );
                  f_setfld( form_id, REPLY_ID, reply_id );

                  obj_find( reply_ocb, "COMMENT.TITLE", &tmp_str );
                  var_set( TITLE_LINE, tmp_str );
                  f_setfld( form_id, TITLE_LINE, tmp_str );

                  obj_update( cc_ocb, "%LINKS.OWNER", &(reply_ocb->oid) );

               }
               else
               {

                  except( Item_Does_Not_Exist, USER_ERROR, new_reply_id,
                                       S_NULL, S_NULL, S_NULL, S_NULL );
                  f_curpos( form_id, NO_FLAGS, REPLY_ID, x, y );
                  rc = NO_ERR;
                  continue;

               }

            }
            else
            {

               except( Item_Does_Not_Exist, USER_ERROR, new_reply_id,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form_id, NO_FLAGS, REPLY_ID, x, y );
               rc = NO_ERR;
               continue;

            }

         }  /* new reply_id ?? */

         var_value = var_get( ADD_ACT );
         var_yn = t_valyn( var_value );
         if( var_yn == TRUE )
         {


            rc = cr_act( cc_ocb, modifier, level, &act_ocb );

            if( rc == NO_ERR )
            {

               activity_attach = TRUE;

               f_setfld( form_id, ADD_ACT,  field_no );
               f_setfld( form_id, GO_TO_SP, field_no );
               var_set(  GO_TO_SP,          field_no );

            }
            else if ( rc == USER_REQUESTED_ABANDON )
            {
               break;
            }
            else
            {

               f_setfld( form_id, ADD_ACT, field_no );
               continue;

            }

         }
         else if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, ADD_ACT, x, y );
            continue;
         }

         if( s_type( var_get( DRAFT ), 0, 0, 'N' ) == 0 )
         {

            s_init( &scratchpad, "A * TEIES $SP" );
            s_cat(  &scratchpad, scratchpad, var_get( DRAFT ), 0, 0 );

         }

    /*******************************************************************\
    |We have figured out which scratchpad to use so now call the scratch|
    |pad for entry of the comment text.  If the user ends the editing   |
    |normally we go on to transfer the text into the CC object.  Then   |
    |we can post the final screen to fill in the header contents.       |
    \*******************************************************************/

         var_value = var_get( GO_TO_SP );
         var_yn = t_valyn( var_value );
         if( var_yn == TRUE )
         {

            tform_rc = t_spad(  scratchpad, &sp_string  );
            edited = TRUE;

         }
         else if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, GO_TO_SP, x, y );
            continue;
         }


         if( t_valyn( var_get( OPTIONS ) ) )
         {

            except( FEATURE_NOT_AVAILABLE, USER_ERROR, S_NULL, S_NULL,
                                               S_NULL, S_NULL, S_NULL );
            f_setfld( form_id, OPTIONS, field_no       );
            f_curpos( form_id, NO_FLAGS, OPTIONS, x, y );
            continue;

         }

    /*******************************************************************\
    |If the user toggels OK_TO_ADD  or our logic decided to set it, try |
    |to do any local validation and then send the Mail to the Master Ser|
    |for final validation.  If a problem occurs we put the form back up |
    |for the user to correct the trouble.                               |
    |                                                                   |
    |If it is not ok to send, then just keep giving the user back the   |
    |same screen and process whatever haas been requested.              |
    \*******************************************************************/


         var_value = var_get( OK_TO_ADD );
         var_yn = t_valyn( var_value );
         if( var_yn == UNEXPECTED_RESPONSE )
         {

            except( Yes_Or_No_Was_Expected, USER_ERROR, var_value,
                                        S_NULL, S_NULL, S_NULL, S_NULL );
            f_curpos( form_id, NO_FLAGS, OK_TO_ADD, x, y );
            continue;

         }
         else if( var_yn == TRUE )
         {

           ocd_new( &cc_ocd, "COMMENT.AUTHOR" );
           obj_rep( cc_ocb, cc_ocd, TM_master->ucb->name, SEG_UPDATE);

           tmp_str = var_get( KEYS_LINE );
           if( s_len(tmp_str) > 0 )
           {

              s_init( &(cc_ocd->name), "COMMENT.KEYS" );
              obj_rep( cc_ocb, cc_ocd, tmp_str, SEG_UPDATE );
                                    /* We should call t_ckkeys() to    */
                                    /* make sure we've correct key fmt */

           }

           tmp_str = var_get( TITLE_LINE );
           if( s_len(tmp_str) > 0 )
           {

              s_init( &(cc_ocd->name), "COMMENT.TITLE" );
              obj_rep( cc_ocb, cc_ocd, tmp_str, SEG_UPDATE );

           }

           tmp_str = var_get( REPLY_ID );
           if( s_len(tmp_str) > 0 )
           {

              s_init( &(cc_ocd->name), "COMMENT.REPLY_ID" );
              obj_rep( cc_ocb, cc_ocd, tmp_str, SEG_UPDATE );

           }

           s_init( &(cc_ocd->name), "COMMENT.SIGNATURE" );
           obj_rep( cc_ocb, cc_ocd,var_get(SIGNATURE),SEG_UPDATE);

           if( edited )
           {

              sp_ocb = obj_opn( (OID *) sp_string, OBJ_READ | OS_FILE  );
              if( OCB_POINTER_OK( sp_ocb ) )
              {

                 ocd_new(     & cc_ocd, "COMMENT.CONTENTS" );
                 rc = ocd_cont( cc_ocb, cc_ocd, sp_ocb   );

              }
              edited = FALSE;

           }

            if ( activity_attach == TRUE )
            {
                 rc = lk_act( act_ocb, S_NULL, level_act, S_NULL );
            }
            if ( rc == NO_ERR )
            {
                 rc = lk_cc( cc_ocb, S_NULL, level, S_NULL );

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

            if( rc == NO_ERR )
            {

               linked = TRUE;

            }

         }  /* IF OK_TO_ADD */
         else   /* otherwise set the OK_TO_ADD field to YES  */
         {

            var_set(  OK_TO_ADD, field_yes   );
            f_setfld( form_id, OK_TO_ADD, field_yes );
            f_curpos( form_id, NO_FLAGS, OK_TO_ADD, x, y );

            var_set(  GO_TO_SP, field_no    );
            f_setfld( form_id, GO_TO_SP, field_no );

         }

      }  /* we do NOT want to quit or abandon */

   }

      /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
      |                                                                 |
      |  At this point 3 things could have happened during the User's   |
      |  interaction process:                                           |
      |  1 ) User said OK TO Create and the CC was properly linked.     |
      |  2 ) The User requested Quit, therefore linked will be False.   |
      |  3 ) There was a "fatal" error and we cannot continue.          |
      |                                                                 |
      \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

   f_close( form_id, NO_FLAGS );
   s_init( &form_id, "1140"   );    /* form opened in crf_gtid() */
   f_close( form_id, NO_FLAGS );

   if( linked )
   {

      s_init( &(cc_ocd->name), "COMMENT.SEQNUM" );
      obj_get( cc_ocb, cc_ocd, &seqnum );
      rc = memb_rc(memb_ocb,S_NULL, 0,seqnum,&(TM_master->ucb->user_id));

      s_init( & form_id, "3152"    );
      s_init( & CONF_ID, "CONF_ID" );
      s_init( &(cc_ocd->name), "CONFERENCE.ID" );
      obj_get( cr_ocb, cc_ocd, &conf_id );
      var_set( CONF_ID, conf_id );

      tform_rc = t_form( form_id, NULL, cc_ocb );
      f_close( form_id, NO_FLAGS );
      var_clr( CONF_ID  );
      s_drop(  &CONF_ID );
      s_drop(  &conf_id );

      t_delsp( sp_string );
      close_flags  = OBJ_COMMIT;

   }
   else
   {

      close_flags = OBJ_DELETE;

   }

   if ( activity_attach == TRUE )
   {

      isv_get( &tmpstr, ISV_CONF, 1 );
      s_cat( &id, tmpstr, conf_id, 0, 0 );
      isv_get( &tmpstr, ISV_CC, 2 );
      s_cat( &id, id, tmpstr, 0, 0 );
      s_init( &(cc_ocd->name), "COMMENT.ID" );
      obj_get( cc_ocb, cc_ocd, &cc_id );
      s_cat( &id, id, cc_id, 0, 0 );
      s_init( &(cc_ocd->name), "ACTIVITY.ID" );
      obj_rep( act_ocb, cc_ocd, cc_id, SEG_UPDATE );

      s_drop( &cc_id );
      s_drop( &id );

      obj_set_flag( act_ocb, close_flags );
      obj_cls(      act_ocb   );

   }

   obj_set_flag( cc_ocb, close_flags );
   obj_cls(      cc_ocb   );

   if( REPLY_OCB_OPENED )
   {

      obj_cls(  reply_ocb  );

   }

   var_clr( OPTIONS    );
   var_clr( TITLE_LINE );
   var_clr( KEYS_LINE  );
   var_clr( SIGNATURE  );
   var_clr( REPLY_ID   );
   var_clr( ADD_ACT    );
   var_clr( DRAFT      );
   var_clr( GO_TO_SP   );
   var_clr( OK_TO_ADD  );

   s_drop( & form_id      );
   s_drop( & conf_id      );
   s_drop( & reply_id     );
   s_drop( & scratchpad   );
   s_drop( & sp_string    );

   s_drop(   &OPTIONS    );
   s_drop(   &TITLE_LINE );
   s_drop(   &KEYS_LINE  );
   s_drop(   &SIGNATURE  );
   s_drop(   &REPLY_ID   );
   s_drop(   &ADD_ACT    );
   s_drop(   &DRAFT      );
   s_drop(   &GO_TO_SP   );
   s_drop(   &OK_TO_ADD  );

}
else
{

   rc = CANNOT_CREATE_OBJECT;

}

}
else
{

   rc = vw_cr( NULL, modifier, (level | CREATING_COMMENT), NULL );

}



   return(rc);
}
