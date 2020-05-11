/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>                             A_DORD.C
<LEVEL>                            ACTIVITY
<AUTHOR>                           Heidi E. Harting
<DATE>                             August 28, 1988

<ERRORS>
                   FEATURE_NOT_AVAILABLE      6005
                   INPUT_EXCEPTION            7904
                   YES_OR_NO_EXPECTED         7904
                   INVALID_CHOICE             7905
                   INVALID_OCB                20022
                   FUNCTION_NOT_SUPPORTED     20222

<FORMS>  A_DORD1M, A_DOABSM, A_DOABSC
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
#include "exc.h"
#include "activity.h"


sint a_dord( act_ocb, attach_type, attach_ocb, return_ocb )

OCB   *act_ocb;
sint  attach_type;
OCB   *attach_ocb;
OCB   **return_ocb;

{

   OCD          *act_ocd          =         NIL,
                *memb_ocd         =         NIL;

   OCB          *memb_ocb         =         NIL,
                *tbl_ocb          =         NIL;

   OID          *memb_oid         =         NIL,
                tbl_oid                        ,
                act_oid                        ;

   sint         rc                =           0,
                xc                =           0,
                R                 =           0,
                rc_spad           =           0,
                stat              =           0,
                x_dash            =           0,
                n_parts           =           0,
                sec_num           =           0,
                x_comma           =           0,
                count             =        9999,
                i                 =           0;

   string       form1             =       S_NULL,
                form2             =       S_NULL,
                read_res          =       S_NULL,
                comma             =       S_NULL,
                all               =       S_NULL,
                new               =       S_NULL,
                req               =       S_NULL,
                reqrd             =       S_NULL,
                option            =       S_NULL,
                a_status          =       S_NULL,
                zero              =       S_NULL,
                vw_rng            =       S_NULL,
                read_abs          =       S_NULL,
                dash              =       S_NULL,
                id                =       S_NULL,
                label             =       S_NULL,
                status            =       S_NULL,
                sta_tus           =       S_NULL,
                act_id            =       S_NULL,
                num_sec           =       S_NULL,
                field_no          =       S_NULL,
                sections          =       S_NULL,
                modifier          =       S_NULL;

   boolean      flag              =        FALSE;

   hword        level             =            0;

   byte         *params           =          NIL,
                required          =         '\0';




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
      s_init( &form1, "A_DORD1M" );
      s_alloc(&modifier, sizeof(OID) );
      s_smp_ul(modifier) = sizeof(OID);
      os_memc(&(act_ocb->oid), s_buffer(modifier), sizeof(OID));

      s_init( &read_abs, "READ_ABS" );
      s_init( &act_id, "ACT_ID" );
      s_init( &a_status, "A_STATUS");
      s_init( &comma, "," );
      s_init( &dash, "-" );
      s_init( &all, "ALL" );
      s_init( &new, "NEW" );
      s_init( &zero, "0" );
      s_init( &read_res, "READ_RES" );
      s_init( &sections, "SECTIONS" );
      s_init( &status, "STATUS");
      isv_get( &field_no, ISV_NO, 1 ); /* DO NOT drop the yes or no*/

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
      obj_find(act_ocb, "ACTIVITY.PART_COUNT", &n_parts);
      s_sitos(&num_sec, n_parts);
      var_set(sections, num_sec);

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

         f_curpos( form1, NO_FLAGS, read_res, 0.0, 0.0 );

         rc = t_form( form1, S_NULL, tbl_ocb );

         if (( rc == USER_REQUESTED_ABANDON )
         || (rc == TM_FORM_NOT_FORMED) || (rc == USER_REQUESTED_QUIT) )
         {
            flag = TRUE;
         }
         else if ( rc == USER_REQUESTED_PRINT )
         {
            flag = TRUE;
            rc = 0;
         }
         else if ( ( rc == USER_REQUESTED_PROCESS )
                || ( rc == USER_REQUESTED_ENTER ) )
         {
            rc = 0;
            if ( (t_valyn( var_get(read_abs))) == -1 )
            {
               except( INPUT_EXCEPTION, USER_ERROR,
                     var_get(read_abs), S_NULL, S_NULL, S_NULL, S_NULL );
               f_curpos( form1, NO_FLAGS, read_abs, 0.0, 0.0 );
               continue;
            }
            if ( (t_valyn( var_get(read_abs))) == 1 )
            {
               t_form( form2, S_NULL, attach_ocb );
               f_close( form2, NO_FLAGS );
               f_setfld( form1, read_abs, field_no );
            }
            s_copy( &vw_rng, var_get( read_res ), 0, 0 );
            s_upper( vw_rng, 0, 0 );
            x_comma = s_first( comma, vw_rng, 0, 0 );
            x_dash = s_first( dash, vw_rng, 0, 0 );

            if ( ( s_comp( vw_rng, all, 0, 0, 'F', &R ) == 0 )
              || ( s_comp( vw_rng, new, 0, 0, 'F', &R ) == 0 )
              || (x_comma > 0) || (x_dash > 0)
              || (s_type(vw_rng, 0, 0, 'N') == 0)            )
            {
               if ( (s_comp(vw_rng, zero, 0, 0, 'F', &R ) == 0)  )
               {
                  continue;
               }
               if ( ( s_comp( vw_rng, all, 0, 0, 'F', &R ) == 0 )
                 || ( s_comp( vw_rng, new, 0, 0, 'F', &R ) == 0 ) )
               {
                  for ( i=1; i <= n_parts; i++ )
                  {
                     rc_spad = a_gtpt(act_ocb, READ, i, attach_type, id);
                     rc = a_crntf(act_ocb, i, READ, attach_type);
                     if (rc_spad == USER_REQUESTED_QUIT)
                     {
                        break;
                     }
                     if ( ( i == n_parts ) && ( stat != ISV_DONE ) )
                     {
                        level = ACT_READ_READ;
                        stat = ISV_UNDONE;
                        rc_act(act_ocb, modifier, level,(byte *)(&stat));
                        s_init(&(memb_ocd->name),
                                              "MEMBERSHIP.MEMBERS.STATUS");
                        stat = (sint) ISV_DONE;
                        obj_rep(memb_ocb, memb_ocd, &stat, SEG_UPDATE);
                     }
                  }
                  var_set(read_res, zero);
                  f_setfld(form1, read_res, zero);
               }
               else  /* need individual sections */
               {
                  if ( (x_comma == 0) && (x_dash == 0) )
                  {
                     s_stosi( &sec_num, vw_rng, 0, 0 );
                     if ((sec_num >= 1)   &&  (sec_num <= n_parts))
                     {
                        a_gtpt(act_ocb, READ, sec_num, attach_type, id);
                        a_crntf(act_ocb, sec_num, READ, attach_type);
                        f_curpos(form1, NO_FLAGS,read_res,0.0,0.0);
                        var_set(read_res, zero);
                        f_setfld( form1, read_res, zero );
                        continue;
                     }
                     else if (sec_num > n_parts)
                     {
                        /* section does not exist */
                        except(INVALID_CHOICE, USER_INFO,
                         var_get(read_res), S_NULL,S_NULL,S_NULL,S_NULL);
                        f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                        var_set(read_res, zero);
                        f_setfld(form1, read_res, zero);
                        continue;
                     }
                  }
                  else if (x_comma == 1)
                  {
                     except(FEATURE_NOT_AVAILABLE, USER_INFO,
                     var_get(read_res), S_NULL,S_NULL,S_NULL,S_NULL);
                     f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                     var_set(read_res, zero);
                     f_setfld(form1, read_res, zero);
                     continue;
                  }
                  else
                  {
                     except(FEATURE_NOT_AVAILABLE, USER_INFO,
                          var_get(read_res),S_NULL,S_NULL,S_NULL,S_NULL);
                     f_curpos(form1,NO_FLAGS,read_res,0.0,0.0 );
                     var_set(read_res, zero);
                     f_setfld(form1, read_res, zero);
                     continue;
                  }
               }
            }
            else
            {
               /* invalid input */
               except(INVALID_CHOICE, USER_ERROR, var_get(read_res),
                                         S_NULL,S_NULL,S_NULL,S_NULL);
               f_curpos( form1, NO_FLAGS, read_res, 0.0, 0.0 );
               var_set(read_res, zero);
               f_setfld(form1, read_res, zero);
            }
         }
         else
         {
            ;
         }
      }                     /* end of while-loop */
      if (OCB_POINTER_OK (tbl_ocb) )
      {
         obj_cls(tbl_ocb);
      }
      f_close(form1, NO_FLAGS);
   }
   if (OCB_POINTER_OK (memb_ocb) )
   {
      obj_cls(memb_ocb);
   }
   s_drop( &modifier);
   s_drop( &form1);
   s_drop( &form2);
   s_drop( &comma );
   s_drop( &dash );
   s_drop( &all );
   s_drop( &vw_rng );
   s_drop( &id );
   var_drp(read_res);
   s_drop( &read_res );
   var_drp(act_id);
   s_drop( &act_id );
   var_drp(read_abs);
   s_drop( &read_abs );
   var_drp(status);
   s_drop( &status );

   return(rc);
}
