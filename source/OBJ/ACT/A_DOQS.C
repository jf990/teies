/************************************************************************
    <DOC>
    <NAME>         a_doqs(OCB *act_ocb, sint attach_type,OCB *attach_ocb)
    <LEVEL>        Activity
    <DATE>         March 11, 1988    March 1, 1989
    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar
    <PARAM>        act_ocb     OCB *  INPUT  The ocb of the activity
                   attach_type sint   INPUT  type of the attached object
                   attach_ocb  OCB *  INPUT  The ocb of the object to
                                             which the activity is
                                             attached.
    <OUTPUT>       Sint.
    <PURPOSE>      This routine will call the routines necessary to DO a
                   Question activity.
    <CALLS>        obj_upd(), obj_find(), obj_get(), obj_all()
                   obj_set_flag(), obj_cls(), ocd_new(), ocd_drp(),
                   ocd_bse(), stosi(), s_init(), s_drop(), s_comp(),
                   s_cat(), s_type(), s_len(), s_sitos(), f_curpos(),
                   f_close(), f_rstfld(), f_setfld(), t_form(),t_valyn(),
                   t_spad(), t_delsp(), var_clr(), var_get(), var_set(),
                   os_memn(), os_memc(), os_mema(), os_memd(), except(),
                   a_crpt(), a_slnot(), a_adact(), isv_match(), isv_get()
    <ERRORS>
                   FEATURE_NOT_AVAILABLE            6005
                   MULTIPLE_RESPONSES_NOT_ALLOWED   6405
                   NOT_ALLOWED_TO_VIEW_RESPONSE     6406
                   PERMISSION_DENIED                6407
                   INPUT_EXCEPTION                  7904
                   YES_OR_NO_EXPECTED               7904
                   INVALID_CHOICE                   7905
                   FUNCTION_NOT_SUPPORTED          20222
    <FORMS>        A_DOQS1M, A_DOABSM, A_DOABSC, A_DOQSAR,
                   and A_ADRES.
    </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "isv.h"
#include "exc.h"
#include "activity.h"


sint a_doqs( act_ocb, attach_type, attach_ocb, return_ocb )

OCB    *act_ocb;
sint   attach_type;
OCB    *attach_ocb;
OCB    **return_ocb;

{
   OCD        *memb_ocd       =         NIL;

   OCB        *p_ocb          =         NIL,
              *memb_ocb       =         NIL;

   OID        *memb_oid       =         NIL,
              act_oid                      ,
              oid1                         ;

   byte       x_byte          =        '\0',
              required        =        '\0',
              byte_x          =        '\0';

   string     form1           =      S_NULL,
              form2           =      S_NULL,
              form3           =      S_NULL,
              form4           =      S_NULL,
              sigs            =      S_NULL,
              conf            =      S_NULL,
              all             =      S_NULL,
              result          =      S_NULL,
              result_sp       =      S_NULL,
              sc_pad           =      S_NULL,
              spad            =      S_NULL,
              sp              =      S_NULL,
              read_abs        =      S_NULL,
              read_res        =      S_NULL,
              zero            =      S_NULL,
              add_res         =      S_NULL,
              new_res         =      S_NULL,
              new             =      S_NULL,
              act_id          =      S_NULL,
              req             =      S_NULL,
              reqrd           =      S_NULL,
              option          =      S_NULL,
              response_id     =      S_NULL,
              vw_res          =      S_NULL,
              comp            =      S_NULL,
              num_resp        =      S_NULL,
              res_loc         =      S_NULL,
              resp            =      S_NULL,
              status          =      S_NULL,
              a_status        =      S_NULL,
              sta_tus         =      S_NULL,
              bad_items       =      S_NULL,
              num             =      S_NULL,
              comma           =      S_NULL,
              dash            =      S_NULL,
              no              =      S_NULL,
              yes             =      S_NULL,
              label           =      S_NULL,
              add             =      S_NULL,
              packet          =      S_NULL,
              period          =      S_NULL,
              id              =      S_NULL,
              id1             =      S_NULL;

   sint       no_res          =           0,
              R               =           0,
              rc_spad         =           0,
              rc              =           0,
              xc              =           0,
              num_bad         =           0,
              sec_num         =           0,
              x_comma         =           0,
              x_dash          =           0,
              added_res       =           0,
              n_resp          =           0,
              rc_add          =           0,
              fld_err         =           0,
              stat            =           0,
              levs            =           0;

   tint       i               =           0;

   hword      level           = ACT_LINK_RESPONSE,
              act_type        =      ISV_QUESTION,
              what_to_get     =  OBJ_NEED_ONE_OID;

   boolean    flag            =       FALSE,
              edited          =       FALSE,
              item            =       FALSE,
              allowed         =       FALSE,
              done            =       FALSE;




   if (attach_type == MAIL_OBJ)
   {
      isv_get(&label, ISV_MMSG, 1);
      s_init( &form2, "A_DOABSM" );
   }
   else if (attach_type == COMMENT_OBJ)
   {
      isv_get(&label, ISV_CC, 1);
      s_init( &form2, "A_DOABSC" );
   }
   else
   {
      rc = FUNCTION_NOT_SUPPORTED;
   }
   if (rc == 0)
   {
      s_init( &form1, "A_DOQS1M" );
      s_init( &read_abs, "READ_ABS" );
      s_init( &read_res, "READ_RES" );
      s_init( &add_res,  "ADD_RES"  );
      s_init( &new_res,  "NEW_RES"  );
      s_init( &res_loc,  "RES_LOC"  );
      s_init( &comp, "COMPOSE" );
      s_init( &resp, "RESP" );
      s_init( &add, "ADD" );
      s_init( &act_id, "ACT_ID");
      s_init( &status, "STATUS");
      s_init( &a_status, "A_STATUS");
      s_init( &spad, "SPAD");
      s_init( &sp, "SP");
      s_init( &sc_pad, "A * RESPONSE $SP");
      s_init( &form3, "A_DOQSAR");
      s_init( &all, "ALL");
      s_init( &new, "NEW");
      s_init( &conf, "CONF");
      s_init( &id1, "ID1");
      s_init( &zero, "0");
      s_init( &comma, ",");
      s_init( &dash, "-");
      s_init( &period, ".");

      isv_get( &no,  ISV_NO,  1 );
      isv_get( &yes, ISV_YES, 1 );


#if 0
      ocd_new(&memb_ocd, "MEMBERSHIP.MEMBERS.VARIABLES.REPLIES_MADE");
#endif
      ocd_new(&memb_ocd, "MEMBERSHIP.MEMBERS.STATUS");
      oid_key(memb_ocd, &(TM_master->ucb->directory->oid));

      memb_oid = ( OID * )os_mema( sizeof (OID) );
      os_memn(memb_oid, sizeof(OID) );
      obj_find( act_ocb, "ACTIVITY.MEMBERS", memb_oid );
      if ( OID_IS_NULL(memb_oid) )
      {

         if (OCB_POINTER_OK (act_ocb) )
         {
            os_memc (&(act_ocb->oid), &act_oid, sizeof(OID) );
            obj_cls(act_ocb);
         }
         act_ocb = t_r_obj(TM_master->MS,
                           OBJ_UPDATE|OBJ_REMOTE, &act_oid);
         if (return_ocb != NIL)
         {
            *return_ocb = act_ocb;
         }
         obj_find( act_ocb, "ACTIVITY.MEMBERS", memb_oid );
      }
      if ( !OID_IS_NULL (memb_oid) )
      {
         memb_ocb = obj_opn(memb_oid, OBJ_UPDATE|OBJ_REMOTE);
      }
      obj_find( act_ocb, "ACTIVITY.ID", &id );

      s_trim(&id, 'B');
      if (attach_type == MAIL_OBJ)
      {
         s_cat(&id, label, id, 0, 0);
      }
      var_set( act_id, id );

      obj_find( act_ocb, "ACTIVITY.REQUIRED", &required );
      s_ctos( &req, required );
      isv_get( &reqrd, ISV_REQUIRED, 0);
      isv_get( &option, ISV_OPTIONAL, 0);
      if ( s_comp( req, reqrd, 1, 1, 'F', &xc ) != 0 )
      {
         var_set(a_status, option);
      }
      else
      {
         var_set(a_status, reqrd);
      }

      while ( !flag )
      {
         if ( OCB_POINTER_OK (memb_ocb) )
         {
            s_init(&(memb_ocd->name), "MEMBERSHIP.MEMBERS.STATUS");
            rc = obj_get( memb_ocb, memb_ocd, &stat);
            if (rc != 0)
            {
               isv_get(&sta_tus, ISV_UNDONE, 0);
            }
            else
            {
               isv_get(&sta_tus, stat, 0);
            }
            var_set(status, sta_tus);
            f_setfld(form1, status, sta_tus);
         }
         else
         {
            rc = INVALID_OCB;
            break;
         }
         obj_find( act_ocb, "ACTIVITY.PART_COUNT", &n_resp );
         s_sitos( &num_resp, n_resp );
         var_set(new_res, num_resp);
         f_setfld(form1, new_res, num_resp);

         f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );

         rc = t_form( form1, S_NULL, NIL );


         if ( ( rc == USER_REQUESTED_ABANDON)
            || (rc == TM_FORM_NOT_FORMED) || (rc == USER_REQUESTED_QUIT))
         {
            flag = TRUE;
         }
         else if ( rc == USER_REQUESTED_PRINT)
         {
            flag = TRUE;
            rc = 0;
         }
         else if ( (rc == USER_REQUESTED_PROCESS)
                || ( rc  == USER_REQUESTED_ENTER ) )
         {
            rc = 0;
            if ( (t_valyn( var_get(read_abs))) == -1 )
            {
               except(INPUT_EXCEPTION, USER_ERROR,
                     var_get(read_abs), S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );
               continue;
            }
            else if ( (t_valyn( var_get(read_abs) )) == 1)
            {
               rc = t_form( form2, S_NULL, attach_ocb );
               f_close( form2, NO_FLAGS );
               f_setfld( form1, read_abs, no );
            }

            if ( ( t_valyn( var_get( add_res ))) == -1 )
            {
               except(INPUT_EXCEPTION, USER_ERROR,
                      var_get(add_res), S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form1, NO_FLAGS, add_res, 0.0, 0.0 );
               continue;
            }
            else if ( (t_valyn (var_get (add_res) ) ) == 1)
            {
               /* need to check if multiple response is allowed */
               /* if not then check if a response has been made */

               obj_find(act_ocb, "ACTIVITY.LIMIT_MAND", &byte_x);
               if (byte_x == 0)
               {

                  s_init( &(memb_ocd->name),
                                  "MEMBERSHIP.MEMBERS.NOTIF_STATUS");
                  rc = obj_get( memb_ocb, memb_ocd, &added_res);
                  if (added_res >= 1)
                  {
                      except(MULTIPLE_RESPONSES_NOT_ALLOWED, USER_INFO,
                                    S_NULL, S_NULL,S_NULL,S_NULL,S_NULL);
                      f_setfld(form1, add_res, no);
                      f_curpos( form1, NO_FLAGS, add_res, 0.0, 0.0 );
                      continue;
                  }
               }
               f_curpos( form3, NO_FLAGS, res_loc, 0.0, 0.0 );
               done = FALSE;
               while ( !done )
               {
                  rc = t_form( form3, S_NULL, NIL );

                  if ( ( rc == USER_REQUESTED_ABANDON )
                    || (rc == TM_FORM_NOT_FORMED) )
                  {
                     flag = TRUE;
                     done = TRUE;
                     continue;
                  }

                  else if ( rc == USER_REQUESTED_QUIT )
                  {
                     done = TRUE;
                     var_set(res_loc, comp);
                     var_set(add, no);
                     rc = 0;
                     continue;
                  }
                  else if ( (rc == USER_REQUESTED_PROCESS)
                      || ( rc == USER_REQUESTED_ENTER )   )
                  {
                     rc = 0;
                     if ( s_comp( var_get(res_loc), comp, 0, 0, 'F', &xc)
                                                                   != 0 )
                     {
                        if ( s_comp(var_get(res_loc), sp, 0, 0, 'F', &xc)
                                                                   != 0 )
                        {
#if 0
                           /* once items work #if will be taken out */
                           rc = obj_item(var_get(res_loc), what_to_get,
                              &oid1, NIL, &bad_items, &num_bad, ANY_OBJ);
                           if (rc != 0)
                           {
                              except( INPUT_EXCEPTION, USER_ERROR,
                              var_get(res_loc),S_NULL,S_NULL,S_NULL,
                                                                 S_NULL);
                              f_curpos(form3,NO_FLAGS,res_loc,0.0,0.0);
                              item = FALSE;
                              continue;
                           }
                           else
                           {
                              item = TRUE;
                              edited = FALSE;
                           }
#endif
                           /* once items work will take out following */
                           if (edited)
                           {
                              f_setfld( form3, res_loc, sp);
                           }
                           else
                           {
                              f_setfld( form3, res_loc, comp);
                           }
                           continue;
                        }
                     }
                     if ((t_valyn (var_get (add) )) == -1 )
                     {
                        except( INPUT_EXCEPTION, USER_ERROR,var_get(add),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
                        f_curpos( form3, NO_FLAGS, add, 0.0, 0.0 );
                        continue;
                     }
                     else            /* all inputs ok */
                     {
                        if ( s_comp( var_get(res_loc), comp, 0, 0, 'F',
                                                              &xc) == 0 )
                        {
                           rc_spad = t_spad( sc_pad, &result_sp );
                           edited = TRUE;
                           item = FALSE;
                           if ((t_valyn (var_get (add) )) == 0 )
                           {
                              f_setfld(form3, res_loc, sp);
                              f_setfld(form3, add, yes);
                              f_curpos( form3, NO_FLAGS, add, 0.0, 0.0 );
                              continue;
                           }
                        }
                     }

                     if (t_valyn (var_get (add)) == 1 )
                     {
                        if (edited)
                        {
                            a_crpt(result_sp, act_ocb, ++n_resp, &p_ocb,
                                                               act_type);
                        }
                        else if (item)
                        {
                           /* need to link item */
                        }
                        else
                        {
                           f_rstfld(form3, NO_FLAGS, add);
                           f_rstfld(form3, NO_FLAGS, res_loc);
                           continue;
                        }
                        done = TRUE;   /* may need to do delsp here */
                        t_delsp( result_sp );
                        /* need to send the objects here */
                        rc = lk_act (p_ocb, num_resp, level, &result);

                        if (rc == 0)
                        {
                           s_init( &(memb_ocd->name),
                                      "MEMBERSHIP.MEMBERS.NOTIF_STATUS");
                           added_res += 1;
                           obj_rep ( memb_ocb, memb_ocd, &added_res,
                                                            SEG_UPDATE );
                           s_init(&(memb_ocd->name),
                                      "MEMBERSHIP.MEMBERS.STATUS");
                           stat = (sint)ISV_DONE;
                           obj_rep ( memb_ocb, memb_ocd, &stat,
                                                            SEG_UPDATE );
                           s_init(&form4, "A_ADRES");
                           obj_find((OCB *)result, "COMMENT.ID",
                                                          &response_id);
                           s_cat(&response_id, period, response_id,0, 0);
                           s_cat(&response_id, id, response_id,0, 0);
                           var_set(id1, response_id);
                           t_form( form4, S_NULL,
                                                         (OCB *)result );
                           f_close( form4, NO_FLAGS );
                           edited = FALSE;
                           item = FALSE;
                           f_rstfld(form3, NO_FLAGS, add);
                           f_rstfld(form3, NO_FLAGS, res_loc);

                        }
                        else
                        {
                           except(rc , USER_WARNING, var_get(add),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
                           continue;
                        }
                     }
                     else
                     {
                        if (s_comp(var_get(res_loc),sp,0,0,'F',&xc) == 0)
                        {
                           if ( (edited) || (item) )
                           {
                              f_setfld(form3, add, yes);
                           }
                           else
                           {
                              f_rstfld(form3, NO_FLAGS, res_loc);
                           }
                        }
                     }
                  }
                  else
                  {
                     continue;
                  }
               }                /* end inner while */
               f_close(form3, NO_FLAGS);
               f_setfld(form1, add_res, no);
            }
            else
            {
               ;
            }
            if (!flag)
            {
               s_copy( &vw_res, var_get( read_res ), 0, 0 );
               s_upper( vw_res, 0, 0 );
               x_comma = s_first( comma, vw_res, 0, 0 );
               x_dash = s_first( dash, vw_res, 0, 0 );

               if ( ( s_comp( vw_res, all, 0, 0, 'F', &R ) == 0 )
                 || ( s_comp( vw_res, new, 0, 0, 'F', &R ) == 0 )
                 || (x_comma > 0) || (x_dash > 0)
                 || (s_type(vw_res, 0, 0, 'N') == 0)            )
               {
                  if ( (s_comp(vw_res, zero, 0, 0, 'F', &R ) == 0)  )
                  {
                     continue;
                  }
                  obj_find( act_ocb, "ACTIVITY.VIEW_BEFORE", &x_byte );

                  if ( (x_byte & BY_PER) == 0 )  /* permission allowed */
                  {
                     if (x_byte & VW_PER)/*allowed to view before resp */
                     {
                        allowed = TRUE;
                     }
                     else
                     {               /* now check if response is made*/
                        s_init( &(memb_ocd->name),
                                     "MEMBERSHIP.MEMBERS.NOTIF_STATUS");
                        rc = obj_get( memb_ocb, memb_ocd, &added_res);
                        if (added_res >= 1)
                        {
                           allowed = TRUE;
                        }
                        else
                        {
                           except(NOT_ALLOWED_TO_VIEW_RESPONSE,USER_INFO,
                                    S_NULL, S_NULL,S_NULL,S_NULL,S_NULL);
                           f_curpos( form1, NO_FLAGS, read_res,0.0,0.0);
                           f_setfld(form1, read_res, zero);
                           continue;
                        }
                     }
                     if (allowed)
                     {

                        if ( ( s_comp(vw_res, all, 0, 0, 'F', &R ) == 0 )
                        || ( s_comp(vw_res, new, 0, 0, 'F', &R ) == 0 ) )
                        {
                           for ( i = 1; i <= n_resp; i++ )
                           {
                              rc =a_gtpt(act_ocb,QUESTION,i,
                                                         attach_type,id);
                              if (rc == USER_REQUESTED_QUIT)
                              {
                                 rc = 0;
                                 break;
                              }
                        /* need to turn on the bits for responses read */
                           }
                           f_setfld(form1, read_res, zero);
                        }
                        else  /* need individual sections */
                        {
                           if ( (x_comma == 0) && (x_dash == 0) )
                           {
                              s_stosi( &sec_num, vw_res, 0, 0 );
                              if ((sec_num >= 1) && (sec_num <= n_resp))
                              {
                                 a_gtpt(act_ocb,QUESTION,
                                                 sec_num,attach_type,id);
#if 0
                                 a_crntf(act_ocb, sec_num, QUESTION,
                                                            attach_type);
#endif
                                 f_curpos(form1, NO_FLAGS,read_res,
                                                                0.0,0.0);
                                 f_setfld( form1, read_res, zero );
                                 continue;
                              }
                              else if (sec_num > n_resp)
                              {
                                except(INVALID_CHOICE,USER_ERROR,
                                    var_get(read_res),
                                            S_NULL,S_NULL,S_NULL,S_NULL);
                                f_curpos(form1,NO_FLAGS,read_res,
                                                                0.0,0.0);
                                 f_setfld(form1, read_res, zero);
                                 continue;
                              }
                           }
                           else if (x_comma == 1)
                           {
                              except(FEATURE_NOT_AVAILABLE, USER_INFO,
                                     var_get(read_res),
                                            S_NULL,S_NULL,S_NULL,S_NULL);
                              f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                              f_setfld(form1, read_res, zero);
                              continue;
                           }
                           else
                           {
                              except(FEATURE_NOT_AVAILABLE, USER_INFO,
                                     var_get(read_res),
                                            S_NULL,S_NULL,S_NULL,S_NULL);
                              f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                              f_setfld(form1, read_res, zero);
                              continue;
                           }
                        }
                     }
                  }
                  else
                  {
                     except(PERMISSION_DENIED, USER_INFO, S_NULL,
                                            S_NULL,S_NULL,S_NULL,S_NULL);
                     f_curpos( form1, NO_FLAGS, read_res, 0.0, 0.0 );
                     f_setfld(form1, read_res, zero);
                     continue;
                  }
               }
               else
               {
                  except(INVALID_CHOICE, USER_ERROR, var_get(read_res),
                                            S_NULL,S_NULL,S_NULL,S_NULL);
                  f_curpos( form1, NO_FLAGS, read_res, 0.0, 0.0 );
                  f_setfld(form1, read_res, zero);
                  continue;
               }
            }
         }
         else
         {
            continue;
         }

      }     /* end outer while */
      f_close(form1, NO_FLAGS);
   }
   if (OCB_POINTER_OK(memb_ocb) )
   {
      obj_cls(memb_ocb);
   }
   s_drop( &form1);
   s_drop( &form2);
   s_drop( &form3);
   s_drop( &resp );
   s_drop( &comp );
   s_drop( &sigs );
   var_clr(read_res);
   s_drop( &read_res );
   var_clr(read_abs);
   s_drop( &read_abs );
   var_clr(add_res);
   s_drop( &add_res );
   var_clr(add);
   s_drop( &add);
   var_clr(new_res);
   s_drop( &new_res );
   var_clr(res_loc);
   s_drop( &res_loc );
   var_clr(status);
   s_drop( &status  );
   s_drop( &zero );
   os_memd(memb_oid);
   return(rc);
}
