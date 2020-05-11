/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                             A_DOSL.C
<LEVEL>                            ACTIVITY
<AUTHOR>                           Heidi E. Harting    Tanmay S. Kumar
<DATE>                             Apr 6, 1988         Aug 1, 1989.

<ERRORS>
              FEATURE_NOT_AVAILABLE                 6005
              ACTIVITY_DONE                         6400
              SELECTION_ACCEPTED                    6409
              SELECTION_LIMIT_REACHED               6410
              INPUT_EXCEPTION                       7904
              YES_OR_NO_EXPECTED                    7904
              INVALID_CHOICE                        7905
              FUNCTION_NOT_SUPPORTED               20222

<FORMS>       A_DOSL1M, A_DOABSM, A_DOABSC
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "vars.h"
#include "isv.h"
#include "obj_name.h"
#include "activity.h"


sint a_dosl( act_ocb, attach_type, attach_ocb, return_ocb )

OCB    *act_ocb;
sint   attach_type;
OCB    *attach_ocb;
OCB    **return_ocb;

{



   OCD        *memb_ocd       =         NIL,
              *tbl_ocd        =         NIL,
              *act_ocd        =         NIL;

   OCB        *p_ocb          =         NIL,
              *user_ocb       =         NIL,
              *tbl_ocb        =         NIL,
              *memb_ocb       =         NIL;

   OID        *memb_oid       =         NIL,
              act_oid                      ,
              tbl_oid                      ,
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
              who             =      S_NULL,
              para            =      S_NULL,
              line            =      S_NULL,
              result          =      S_NULL,
              result_sp       =      S_NULL,
              s_pad           =      S_NULL,
              spad            =      S_NULL,
              sp              =      S_NULL,
              read_abs        =      S_NULL,
              read_res        =      S_NULL,
              zero            =      S_NULL,
              select          =      S_NULL,
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
              num             =      S_NULL,
              comma           =      S_NULL,
              dash            =      S_NULL,
              no              =      S_NULL,
              yes             =      S_NULL,
              label           =      S_NULL,
              add             =      S_NULL,
              packet          =      S_NULL,
              rc_spad         =      S_NULL,
              period          =      S_NULL,
              id              =      S_NULL,
              id1             =      S_NULL;

   sint       no_res          =           0,
              R               =           0,
              rc              =           0,
              xc              =           0,
              count           =        9999,
              selec           =           0,
             *line_num        =         NIL,
              sec_num         =           0,
              x_comma         =           0,
              x_dash          =           0,
              added_sel       =           0,
              n_resp          =           0,
              rc_add          =           0,
              fld_err         =           0,
              stat            =           0,
              levs            =           0;

   tint       i               =           0;

   hword      level           = ACT_LINK_SELECTION,
              what_to_get     =  OBJ_NEED_ONE_OID;

   boolean    flag            =       FALSE,
              edited          =       FALSE,
              item            =       FALSE,
              allowed         =       FALSE,
              done            =       FALSE;


   string        params           =        S_NULL;



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
      s_init( &form1, "A_DOSL1M" );
      s_init( &read_abs, "READ_ABS" );
      s_init( &read_res, "READ_RES" );
      s_init( &select,  "SELECT"  );
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
      s_init( &s_pad, "A * RESPONSE $SP");
      s_init( &form3, "A_DOQSAR");
      s_init( &all, "ALL");
      s_init( &new, "NEW");
      s_init( &conf, "CONF");
      s_init( &id1, "ID1");
      s_init( &zero, "0");
      s_init( &comma, ",");
      s_init( &dash, "-");
      s_init( &period, ".");
      s_init( &para, "<p>");

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
      obj_find(act_ocb, "ACTIVITY.PART_COUNT", &n_resp );
      s_sitos(&num_resp, n_resp );
      var_set(new_res, num_resp);

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
         ocd_new( &act_ocd, "ACTIVITY.PART_LIST.LOCATION" );
         s_alloc(&(act_ocd->key), 4);
         s_smp_ul(act_ocd->key) = 4;
         os_memc( &count, s_buffer( act_ocd->key ), 4 );
         obj_get( act_ocb, act_ocd, &tbl_oid );
         tbl_ocb = obj_opn(&tbl_oid, OBJ_UPDATE | OBJ_REMOTE );
         if ( !OCB_POINTER_OK (tbl_ocb) )
         {
            rc = INVALID_OCB;
            break;
         }

         f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );

         rc = t_form( form1, S_NULL, tbl_ocb );

         if ( ( rc == USER_REQUESTED_ABANDON)
         || (rc == USER_REQUESTED_QUIT) || (rc == TM_FORM_NOT_FORMED) )
         {
            flag = TRUE;
         }
         else if ( rc == USER_REQUESTED_PRINT )
         {
            flag = TRUE;
            rc = 0;
         }
         else if ( (rc == USER_REQUESTED_PROCESS)
                || ( rc == USER_REQUESTED_ENTER) )
         {
            rc = 0;
            if ( (t_valyn( var_get(read_abs))) == -1 )
            {
               except(INPUT_EXCEPTION, USER_ERROR,
                     var_get(read_abs), S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );
               continue;
            }
            if ( (t_valyn( var_get(read_abs) )) == 1)
            {
               t_form( form2, S_NULL, attach_ocb );
               f_close( form2, NO_FLAGS );
               f_setfld( form1, read_abs, no );
            }
            s_copy( &vw_res, var_get( read_res ), 0, 0 );
            s_upper( vw_res, 0, 0 );
            x_comma = s_first( comma, vw_res, 0, 0 );
            x_dash = s_first( dash, vw_res, 0, 0 );

            if ( ( s_comp( vw_res, all, 0, 0, 'F', &R ) == 0 )
              || ( s_comp( vw_res, new, 0, 0, 'F', &R ) == 0 )
              || (x_comma > 0) || (x_dash > 0)
              || (s_type(vw_res, 0, 0, 'N') == 0)            )
            {

               if ( ( s_comp(vw_res, all, 0, 0, 'F', &R ) == 0 )
               || ( s_comp(vw_res, new, 0, 0, 'F', &R ) == 0 ) )
               {
                  for ( i = 1; i <= n_resp; i++ )
                  {
                     rc = a_gtpt( act_ocb, SELECTION, i, attach_type,id);
                     if (rc == USER_REQUESTED_QUIT)
                     {
                        rc = 0;
                        break;
                     }
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
                        a_gtpt(act_ocb,SELECTION,sec_num,attach_type,id);
                        f_curpos(form1, NO_FLAGS,read_res, 0.0, 0.0);
                        f_setfld( form1, read_res, zero );
                        continue;
                     }
                     else if (sec_num > n_resp)
                     {
                        except(INVALID_CHOICE, USER_ERROR,
                          var_get(read_res),S_NULL,S_NULL,S_NULL,S_NULL);
                        f_curpos(form1,NO_FLAGS,read_res, 0.0,0.0);
                        f_setfld(form1, read_res, zero);
                        continue;
                     }
                  }
                  else if (x_comma == 1)
                  {
                     except(FEATURE_NOT_AVAILABLE, USER_INFO,
                          var_get(read_res),S_NULL,S_NULL,S_NULL,S_NULL);
                     f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                     f_setfld(form1, read_res, zero);
                     continue;
                  }
                  else
                  {
                     except(FEATURE_NOT_AVAILABLE, USER_INFO,
                          var_get(read_res),S_NULL,S_NULL,S_NULL,S_NULL);
                     f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                     f_setfld(form1, read_res, zero);
                     continue;
                  }
               }
            }
            else
            {
               except(INVALID_CHOICE,USER_ERROR, var_get(read_res),
                                         S_NULL,S_NULL,S_NULL,S_NULL);
               f_curpos( form1, NO_FLAGS, read_res, 0.0, 0.0 );
               f_setfld(form1, read_res, zero);
               continue;
            }

            if ( s_type( var_get(select), 0, 0, 'N' ) != 0 )
            {
               except(INVALID_CHOICE, USER_ERROR,
                     var_get(select), S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );
               continue;
            }

            else
            {
               /*  check if selection is in limits       */
               s_stosi(&selec, var_get(select), 0, 0);
               if ( (selec > 0) && (selec <= n_resp) )
               {
                  /* check if selection limit is reached   */

                  obj_find(act_ocb, "ACTIVITY.LIMIT_MAND", &byte_x);

                  s_init( &(memb_ocd->name),
                                  "MEMBERSHIP.MEMBERS.NOTIF_STATUS");
                  rc = obj_get( memb_ocb, memb_ocd, &added_sel);
                  if (added_sel >= byte_x)
                  {
                     except(SELECTION_LIMIT_REACHED, USER_ERROR, S_NULL,
                                       S_NULL,S_NULL,S_NULL,S_NULL);
                     f_setfld(form1, select, zero);
                     f_curpos( form1, NO_FLAGS, select, 0.0, 0.0 );
                  }
                  else
                  {
                  /* check if the selection can be made and do the upd */
                     rc = lk_act(act_ocb,var_get(select), level,&params);
                     if (rc == 0)
                     {
                        /* since selection was success do local receive*/

                        oid_key(act_ocd, &(act_ocb->oid) );
                        rc_act(act_ocb, act_ocd->key, level,
                                                        (byte *)&params);
                        ocd_new( &tbl_ocd, "COMMENT.CONTENTS.TEXT" );
                        s_alloc(&(tbl_ocd->key),4);
                        s_smp_ul(tbl_ocd->key) = 4;
                        line_num = (sint *)s_buffer(tbl_ocd->key);
                        *(line_num) = selec;
                        obj_get(tbl_ocb, tbl_ocd, &line);
                        s_copy(&line, line, 1, 32);
                        user_ocb = t_author();
                        gt_name(user_ocb,S_NULL,NICKNAME_ONLY,&who);
                        s_cat(&line, line, who, 0, 0);
                        s_cat(&line, line, para, 0, 0);
                        obj_rep(tbl_ocb, tbl_ocd,line,SEG_UPDATE);
                        f_close(form1, NO_FLAGS);

                        /* need to update local table    object and */
                        /* local membership object                  */
                        s_init( &(memb_ocd->name),
                                      "MEMBERSHIP.MEMBERS.NOTIF_STATUS");
                        added_sel += 1;
                        obj_rep ( memb_ocb, memb_ocd, &added_sel,
                                                            SEG_UPDATE );
                        s_init(&(memb_ocd->name),
                                      "MEMBERSHIP.MEMBERS.STATUS");
                        stat = (sint)ISV_DONE;
                        obj_rep ( memb_ocb, memb_ocd, &stat, SEG_UPDATE);
                        s_init(&(memb_ocd->name),
                                      "MEMBERSHIP.MEMBERS.WINDOW");
                        a_addsel(memb_ocb, var_get(select), level, 0,
                                                              memb_ocd);
                        except(ACTIVITY_DONE, USER_INFO, S_NULL,
                                       S_NULL,S_NULL,S_NULL,S_NULL);
                        f_setfld(form1, select, zero);
                        f_curpos( form1, NO_FLAGS, select, 0.0, 0.0 );

                     }
                     else
                     {
                        except(rc, USER_WARNING, S_NULL,
                                       S_NULL,S_NULL,S_NULL,S_NULL);
                        f_setfld(form1, select, zero);
                        f_curpos( form1, NO_FLAGS, select, 0.0, 0.0 );
                        continue;
                     }
                  }
               }
               else if (selec == 0)
               {
                  continue;
               }
               else
               {
                  except(INVALID_CHOICE, USER_ERROR,
                       var_get(select), S_NULL, S_NULL, S_NULL, S_NULL );
                  f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );
                  continue;
               }
            }
         }
         else
         {
            ;
         }

      }     /* end outer while */
      if (OCB_POINTER_OK(tbl_ocb) )
      {
         obj_cls(tbl_ocb);
      }
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
   var_clr(select);
   s_drop( &select );
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
