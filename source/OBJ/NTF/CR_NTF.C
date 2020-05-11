/************************************************************************
     <DOC>
     <NAME>    cr_ntf(OCB *ocb, string modifier, hword level, OCB
                                                            **params)
     <AUTHOR>  Tanmay S Kumar
     <DATE>    July 27, 1988
     <LEVEL>   OBJ
     <PARAM>   ocb         *OCB      INPUT   Object of notification.
     <PARAM>   modifier    string    INPUT   This parameter may have
                                             the names to send the
                                             notification to.
     <PARAM>   level       hword     INPUT   This shows whether the
                                             Notification is of INTER-
                                             ACTIVE OR AUTOMATIC type
     <PARAM>   params     *byte      INPUT   If the OCB passed is NIL
                                             the new OCB allocated is
                                             passed back if this param
                                             is not NIL>
     <PURPOSE> Create Notifications of INTERACTIVE or AUTOMATIC type
               For Interactive type of notifications if there is a
               user error the cursor is set to that field where the
               error occured if it can be resolved> Both the notific-
               ation types are linked by sending the notification to
               the Master server. The automatic notification types do
               not have any errors. If linking of the Interactive
               notifications is not possible this routine returns an
               error code depending on which certain actions are taken
     <OUTPUT>  Returns 0 if successful otherwise returns an error code
               depending on the error.
     <CALLS>   obj_all(), s_init(), obj_update(), t_form(), t_ifquit()
               obj_set_flag(), obj_cls(), obj_del(), s_drop(),
               ocd_new(), obj_rep(), s_stosi(), f_setfld(), var_set(),
               except(), f_curpos(), f_close(), s_alloc(), os_memn(),
               s_smp_ul(), lk_ntf(), s_len(), s_cat(), ocd_drp(),
               and some debugging routines.
               It also calls a macro called OCB_POINTER_OK().
     <ERRORS>
               INPUT_EXCEPTION            7904
               YES_OR_NO_EXPECTED         7904
               INVALID_CHOICE             7905
               NO_ADDRESSES_SPECIFIED     6368
               NO_DATA_PROVIDED          20026

     <LIMITS>  As of Now for Interactive  notifications we assume that
               the OCB pased is NIL. If the level is not set to either
               INTERACTIVE or AUTOMATIC nothing is done.
     </DOC>
************************************************************************/
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
#include "bse.h"


sint     cr_ntf( ocb, modifier, level, params )

OCB               *ocb;
string             modifier;
hword              level;
OCB              **params;


{

 string  state1        = S_NULL,
         state2        = S_NULL,
         state3        = S_NULL,
         TO            = S_NULL,
         ASSOCIATED    = S_NULL,
         APPEND_TO_ITM = S_NULL,
         INC_AUTHOR    = S_NULL,
         CONTENTS1     = S_NULL,
         CONTENTS2     = S_NULL,
         CONTENTS3     = S_NULL,
         TYPE          = S_NULL,
         STATUS        = S_NULL,
         CATEGORY      = S_NULL,
         AGREE         = S_NULL,
         DISAGREE      = S_NULL,
         DESIRABLE     = S_NULL,
         UNDESIRABLE   = S_NULL,
         APPROVE       = S_NULL,
         DISAPPROVE    = S_NULL,
         FEASIBLE      = S_NULL,
         INFEASIBLE    = S_NULL,
         VALID         = S_NULL,
         INVALID       = S_NULL,
         APPLAUD       = S_NULL,
         BOO           = S_NULL,
         UNCLEAR       = S_NULL,
         INTERESTED_IN = S_NULL,
         WILL_HANDLE   = S_NULL,
         HOLD_OFF      = S_NULL,
         LINE1         = S_NULL,

         YorN          = S_NULL,
         line          = S_NULL,
         line1         = S_NULL,
         field         = S_NULL,
         packet        = S_NULL,
         result_str    = S_NULL,
         blank         = S_NULL,

         n_prefix      = S_NULL,
         m_prefix      = S_NULL,
         c_prefix      = S_NULL,
         g_prefix      = S_NULL,
         cc_prefix     = S_NULL;

 OCB     *notif_ocb    =    NIL;

 OCD     *notif_ocd    =    NIL;

 sint    rc            =      0,
         xc            =      0,
         bse_value     =      0,
         temp          =      0,
         *line_num     =    NIL;

 boolean flag          =  FALSE,
         linked        =  FALSE;

 real    x             =    0.0,
         y             =    0.0;

 #D_begin(cr_ntf, OBJ)
 #D_show ('Mod=x%x; level=%d; param=x%x.','modifier,level,params');
 #D_str  ('modifier','If not S_NULL then send to.');


if (level == 0 || level == INTERACTIVE_CREATION )
{

   #D_say('assuming ocb is NIL - allocating new notif_ocb');
   notif_ocb = obj_all(NOTIF_OBJ, LOCAL_OBJECT, NIL);


   if (OCB_POINTER_OK (notif_ocb))
   {



      /* set the owner link to the current user */

      obj_update(notif_ocb, "%LINKS.OWNER",&(TM_master->ucb->user_id));

      s_init(   &state1,         "3171"       );
      s_init(   &state2,         "3170"       );
      s_init(   &AGREE, "Agree with content; ");
      s_init(   &DISAGREE, "Disagree with content; ");
      s_init(   &DESIRABLE, "Desirable proposal; ");
      s_init(   &UNDESIRABLE, "Undesirable proposal; ");
      s_init(   &APPROVE, "Approve action; ");
      s_init(   &DISAPPROVE, "Disapprove with action; ");
      s_init(   &FEASIBLE, "Feasible concept; ");
      s_init(   &INFEASIBLE, "Unfeasible concept; ");
      s_init(   &VALID, "Valid argument; ");
      s_init(   &INVALID, "Invalid argument; ");
      s_init(   &APPLAUD, "Applaud content; ");
      s_init(   &BOO, "Boo content; ");
      s_init(   &UNCLEAR, "Unclear content; ");
      s_init(   &INTERESTED_IN, "Interested in subject; ");
      s_init(   &WILL_HANDLE, "Will handle task; ");
      s_init(   &HOLD_OFF, "hold off action; " );

      s_init(   &TO, "TO" );
      s_init(   &ASSOCIATED, "ASSOCIATED" );
      s_init(   &APPEND_TO_ITM, "APPEND_TO_ITM" );
      s_init(   &INC_AUTHOR, "INC_AUTHOR" );
      s_init(   &CONTENTS1, "CONTENTS1" );
      s_init(   &CONTENTS2, "CONTENTS2" );
      s_init(   &CONTENTS3, "CONTENTS3" );
      s_init(   &TYPE, "TYPE" );
      s_init(   &STATUS, "STATUS" );
      s_init(   &CATEGORY, "CATEGORY" );

      s_init(   &YorN,      "yes or no" );
      ocd_new(&notif_ocd, "NOTIFICATION.TYPE" );

      while (!flag)
      {
         #D_say('calling form 3171');
         rc = 0;
         flag = FALSE;

         f_curpos(state1, NO_FLAGS, TYPE, x, y);
         rc = t_form( state1, S_NULL, NIL);

         if ( ( rc == USER_REQUESTED_ENTER )
         || ( rc == USER_REQUESTED_PROCESS ) )
         {

            rc = 0;
            s_stosi(&temp, var_get(TYPE), 0, 0);
            #D_show('type = %d ','temp');


            switch (temp)
            {
               /* user wants to create his own notification */

               case 0:  var_clr(CONTENTS1);
                        var_clr(CONTENTS2);
                        var_clr(CONTENTS3);
                        break;
               case 1:  f_setfld( state2, CONTENTS1, AGREE);
                        var_set(CONTENTS1, AGREE);
                        break;
               case 2:  f_setfld( state2, CONTENTS1, DISAGREE);
                        var_set(CONTENTS1, DISAGREE);
                        break;
               case 3:  f_setfld( state2, CONTENTS1, DESIRABLE);
                        var_set(CONTENTS1, DESIRABLE);
                        break;
               case 4:  f_setfld( state2, CONTENTS1, UNDESIRABLE);
                        var_set(CONTENTS1, UNDESIRABLE);
                        break;
               case 5:  f_setfld( state2, CONTENTS1, APPROVE);
                        var_set(CONTENTS1, APPROVE);
                        break;
               case 6:  f_setfld( state2, CONTENTS1, DISAPPROVE);
                        var_set(CONTENTS1, DISAPPROVE);
                        break;
               case 7:  f_setfld( state2, CONTENTS1, FEASIBLE);
                        var_set(CONTENTS1, FEASIBLE);
                        break;
               case 8:  f_setfld( state2, CONTENTS1, INFEASIBLE);
                        var_set(CONTENTS1, INFEASIBLE);
                        break;
               case 9:  f_setfld( state2, CONTENTS1, VALID);
                        var_set(CONTENTS1, VALID);
                        break;
               case 10: f_setfld( state2, CONTENTS1, INVALID);
                        var_set(CONTENTS1, INVALID);
                        break;
               case 11: f_setfld( state2, CONTENTS1, APPLAUD);
                        var_set(CONTENTS1, APPLAUD);
                        break;
               case 12: f_setfld( state2, CONTENTS1, BOO);
                        var_set(CONTENTS1, BOO);
                        break;
               case 13: f_setfld( state2, CONTENTS1, UNCLEAR);
                        var_set(CONTENTS1, UNCLEAR);
                        break;
               case 14: f_setfld( state2, CONTENTS1, INTERESTED_IN);
                        var_set(CONTENTS1, INTERESTED_IN);
                        break;
               case 15: f_setfld( state2, CONTENTS1, WILL_HANDLE);
                        var_set(CONTENTS1, WILL_HANDLE);
                        break;
               case 16: f_setfld( state2, CONTENTS1, HOLD_OFF);
                        var_set(CONTENTS1, HOLD_OFF);
                        break;
               default:
                        #D_say('Invalid Notification Type Choosen');
                        f_curpos(state1, NO_FLAGS, TYPE, x, y);
                        except(INVALID_CHOICE,USER_ERROR,var_get(TYPE),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
                        continue;
                        break;
            }                    /* end switch */

            #D_say('allocating new OCD');
            s_init( &(notif_ocd->name), "NOTIFICATION.TO" );
            #D_say('calling obj_rep');
            obj_rep(notif_ocb, notif_ocd, &temp, SEG_HOLD);
         }                       /* end     if */
         else if ( ( rc == USER_REQUESTED_QUIT )
         || ( rc == USER_REQUESTED_ABANDON ) || (rc==TM_FORM_NOT_FORMED))
         {
            flag = TRUE;
            linked = TRUE;
            continue;
         }
         else
         {
            continue;
         }

         if ((modifier != S_NULL)&& (rc == 0) )
         {
            f_setfld(state2, TO, modifier);
            var_set(TO, modifier);
         }

         linked = FALSE;

         while (!linked)
         {

            rc = t_form( state2, S_NULL, NIL);

            if ( ( rc == USER_REQUESTED_PROCESS)
               || ( rc == USER_REQUESTED_ENTER ) )
            {
               rc = 0;
               if ( (s_len((string)var_get(TO)) == 0) &&
                    (s_len((string)var_get(ASSOCIATED)) == 0) )
               {

                  #D_say('No one to send Notification to');
                  except( NO_ADDRESSES_SPECIFIED, USER_ERROR, S_NULL,
                           S_NULL, S_NULL, S_NULL, S_NULL );
                  continue;
               }
               if ( (s_len((string)var_get(CONTENTS1)) == 0)
                          && (s_len((string)var_get(CONTENTS2)) == 0)
                          && (s_len((string)var_get(CONTENTS3)) == 0))
               {
                  #D_say('No contents to send');
                  except( NO_DATA_PROVIDED, USER_ERROR, S_NULL,
                           S_NULL, S_NULL, S_NULL, S_NULL );
                  continue;
               }
               s_init( &(notif_ocd->name), "NOTIFICATION.TO" );
               obj_rep( notif_ocb, notif_ocd, var_get(TO), SEG_HOLD);

               s_init( &(notif_ocd->name), "NOTIFICATION.ASSOCIATED" );
               obj_rep(notif_ocb,notif_ocd,(string)var_get(ASSOCIATED),
                                                               SEG_HOLD);

               s_init( &(notif_ocd->name), "NOTIFICATION.AUTHOR" );
               obj_rep(notif_ocb,notif_ocd,TM_master->ucb->name,
                                                           SEG_HOLD);
               s_init( &(notif_ocd->name), "NOTIFICATION.INC_AUTHOR" );
               obj_rep(notif_ocb,notif_ocd,(string)var_get(INC_AUTHOR),
                                                           SEG_UPDATE);
               s_init( &(notif_ocd->name), "NOTIFICATION.CONTENTS.TEXT");
               s_alloc(&(notif_ocd->key), BSE_MAX_KEY_SIZE);
               os_memn(s_buffer(notif_ocd->key), BSE_MAX_KEY_SIZE);
               s_smp_ul(notif_ocd->key) = BSE_MAX_KEY_SIZE;
               line_num = (sint *) s_buffer(notif_ocd->key);
               bse_value = B_RFIRST | B_PRINTABLE;
               #D_show('bse_value = %x', 'bse_value');

               if (s_len((string)var_get(CONTENTS1)) > 0)
               {

                *(line_num) += 1;
                #D_mdump('s_buffer(notif_ocd->key)', 'BSE_MAX_KEY_SIZE');
                s_init( &(notif_ocd->name),"NOTIFICATION.CONTENTS.TEXT");
                obj_rep(notif_ocb,notif_ocd,
                                    (string)var_get(CONTENTS1),SEG_HOLD);
                s_init( &(notif_ocd->name), "NOTIFICATION.CONTENTS.BSE");
                obj_rep(notif_ocb, notif_ocd, &bse_value, SEG_UPDATE);

               }

               bse_value = B_RCONT | B_PRINTABLE;

               if (s_len((string)var_get(CONTENTS2)) > 0)
               {

                *(line_num) += 1;
                #D_mdump('s_buffer(notif_ocd->key)', 'BSE_MAX_KEY_SIZE');
                s_init(&(notif_ocd->name), "NOTIFICATION.CONTENTS.TEXT");
                obj_rep(notif_ocb,notif_ocd,
                                    (string)var_get(CONTENTS2),SEG_HOLD);
                s_init( &(notif_ocd->name), "NOTIFICATION.CONTENTS.BSE");
                obj_rep(notif_ocb, notif_ocd, &bse_value, SEG_UPDATE);

               }

               if (s_len((string)var_get(CONTENTS3)) > 0)
               {

                *(line_num) += 1;
                #D_mdump('s_buffer(notif_ocd->key)', 'BSE_MAX_KEY_SIZE');
                s_init(&(notif_ocd->name), "NOTIFICATION.CONTENTS.TEXT");
                obj_rep(notif_ocb,notif_ocd,
                                    (string)var_get(CONTENTS3),SEG_HOLD);
                s_init( &(notif_ocd->name), "NOTIFICATION.CONTENTS.BSE");
                obj_rep(notif_ocb, notif_ocd, &bse_value, SEG_UPDATE);

               }


               /* we have to call lk_ntf() to do all the linking */

               rc =lk_ntf(notif_ocb,packet,level,(byte **)(&result_str));
               #D_ocb(notif_ocb);
               switch( rc )
               {

                  case  0                     :
                                               linked     = TRUE;
                                               flag = TRUE;
                                               /* We are done ! */
                                               break;

                  case  NAME_NOT_FOUND        :
                  case  NOT_ALLOWED_TO_SEND_TO:
                                        s_init(&field,"TO");
                                        break;

                  case  BAD_ASSOCIATION       :
                               s_init(&field,"ASSOCIATED");
                               break;

                  default                     :
                                               linked = FALSE;

               }
               if (s_len( field ) > 0)
               {

                  f_curpos( state2, NO_FLAGS, field, x, y );
               }
               if (rc > 0)
               {

                  #D_say( 'Link returned an error.' );
                  except( rc, USER_ERROR, result_str,
                           S_NULL, S_NULL, S_NULL, S_NULL );
                  /* need to set level so as to let the master server */
                  /* know that we are just sending the corrected info */
                  /* to it.   e.g     level = 99;                     */
               }
           }
           else if ( rc == USER_REQUESTED_QUIT )

           {
               /* If QUIT  clean up and go back to previous form */

               #D_say( 'User requested QUIT.' );
               rc = USER_REQUESTED_QUIT;
               linked = TRUE;
           }
           else if ( rc == USER_REQUESTED_ABANDON )
           {
               /* If  ABANDON, clean up and return */

               #D_say( 'User requested ABANDON.' );
               linked = TRUE;
               flag   = TRUE;
           }
           else
           {
               continue;
           }
         }      /* end while loop */
         f_close(state2, NO_FLAGS);

      }  /* end while outer loop */
      f_close(state1, NO_FLAGS);
      ocd_drp(&notif_ocd);

      if ((linked) && (rc == 0) )
      {
         #D_say('Notification linked - showing result form');
         s_init(&state3, "1172");
         s_init( &LINE1, "LINE1" );
         s_init(&blank, " ");
         isv_get(&line, ISV_NOTIF, 1);
         s_copy(&line1, line, 0, 0);
         s_cat(&line1, line1, blank, 0, 0);
         s_cat(&line1, line1, result_str, 0, 0);
         s_trim(&line1, 'B');
         var_set(LINE1, line1);
         t_form(state3, S_NULL, notif_ocb);
         f_close(state3, NO_FLAGS);
         s_drop( &line1 );
         s_drop( &LINE1 );
         s_drop( &blank );
         s_drop( &state3 );

         if (  params != NIL  )
         {

            * params = notif_ocb;

         }
      }
      if ( rc != 0 )
      {

         obj_set_flag(  notif_ocb, OBJ_DELETE  );

      }
      obj_cls(  notif_ocb );
      s_drop( & state1    );
      s_drop( & state2    );
      s_drop( & field     );
      s_drop( & packet    );
      s_drop( & YorN      );
      var_clr(TO);
      s_drop( &TO  );
      var_clr(ASSOCIATED);
      s_drop( &ASSOCIATED);
      var_clr(APPEND_TO_ITM);
      s_drop( &APPEND_TO_ITM);
      var_clr(INC_AUTHOR);
      s_drop( &INC_AUTHOR);
      var_clr(CONTENTS1);
      s_drop( &CONTENTS1);
      var_clr(CONTENTS2);
      s_drop( &CONTENTS2);
      var_clr(CONTENTS3);
      s_drop( &CONTENTS3);
      var_clr(TYPE);
      s_drop( &TYPE);
      var_clr(STATUS);
      s_drop( &STATUS);
      var_clr(CATEGORY);
      s_drop( &CATEGORY);

      s_drop( &AGREE   );
      s_drop( &DISAGREE    );
      s_drop( &DESIRABLE   );
      s_drop( &UNDESIRABLE );
      s_drop( &APPROVE     );
      s_drop( &DISAPPROVE  );
      s_drop( &FEASIBLE    );
      s_drop( &INFEASIBLE  );
      s_drop( &VALID       );
      s_drop( &INVALID     );
      s_drop( &APPLAUD     );
      s_drop( &BOO         );
      s_drop( &UNCLEAR     );
      s_drop( &INTERESTED_IN );
      s_drop( &WILL_HANDLE );
      s_drop( &HOLD_OFF    );

   } /* end if OCB_POINTER_OK */
   else
   {
      rc = CANNOT_CREATE_OBJECT;
      #D_say( 'Cannot allocate an object.' );

      except( rc, USER_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
   }

}  /* end of if statement for level= interactive */
else if ( level == AUTOMATIC_CREATION )
{

   #D_say( 'Automatic generation of confirmation by object type' );
   if (OCB_POINTER_OK (ocb))
   {
      #D_ocb(ocb);
      rc = lk_ntf(ocb, modifier, level, (byte **)params);
   }
   else
   {
      rc = INVALID_OCB;
      #D_say( 'passed bad/invalid ocb' );
      except( rc, USER_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL );
   }
}

#D_return(rc)

}
