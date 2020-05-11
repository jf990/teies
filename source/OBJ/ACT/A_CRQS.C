/************************************************************************
    <DOC>
    <NAME>         a_crqs(OCB *atch_ocb, OID *act_oid)
    <LEVEL>        Activity
    <DATE>         March 11, 1988    May 8, 1989
    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar
    <PARAM>        atch_ocb   OCB *   INPUT  The ocb of the object to
                                             which the activity will be
                                             attached.
                   act_oid    OID *   OUTPUT oid of activity created
    <OUTPUT>       Sint.
    <PURPOSE>      This routine will traverse the screens necessary and
                   control the variables needed to create a QUESTION
                   activity
    <CALLS>        obj_opn(), obj_all(), obj_set_flag(), obj_cls(),
                   ocd_new(), ocd_drp(), ocd_bse(), stosi(), s_init(),
                   s_drop(), s_comp(), s_cat(), s_type(), s_first(),
                   f_curpos(), f_close(), t_form(), t_valyn(), t_spad(),
                   t_delsp(), var_clr(), var_get(), os_memc(), except(),
                   sizeof(), a_slnot(), a_adact(), isv_match().
    <FORMS>        A_CRQSMC
    <ERRORS>
                   ACCESS_DENIED_FOR_OBJECT  7700
                   INPUT_EXCEPTION           7904
                   YES_OR_NO_EXPECTED        7904
                   INVALID_CHOICE            7905
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

sint a_crqs( atch_ocb, act_oid )

OCB     *atch_ocb;
OID     *act_oid;

{
   OCB       *act_ocb       =          NIL,
             *sp_ocb        =          NIL;

   OCD       *atch_ocd      =          NIL;

   OID       oid1                         ,
             oid2                         ;

   string    form           =       S_NULL,
             comp           =       S_NULL,
             sigs           =       S_NULL,
             sign           =       S_NULL,
             sp             =       S_NULL,

             ab_loc         =       S_NULL,
             num_parts      =       S_NULL,
             limit_mand     =       S_NULL,
             signature      =       S_NULL,
             view_before    =       S_NULL,
             by_per         =       S_NULL,
             notf           =       S_NULL,
             sp_priv        =       S_NULL,
             sequed         =       S_NULL,
             to_do          =       S_NULL,
             okto_cr        =       S_NULL,

             options        =       S_NULL,
             reg_opt        =       S_NULL,
             field_no       =       S_NULL,
             field_yes      =       S_NULL,
             stat           =       S_NULL,
             status         =       S_NULL,
             sta_tus        =       S_NULL,
             scratchpad     =       S_NULL,
             result         =       S_NULL,
             bad_items      =       S_NULL;

   sint      xc             =            0,
             rc             =            0,
             rc_spad        =            0,
             levs           =            0,
             num_bad        =            0,
             rc_notf        =            0;

   boolean   flag           =        FALSE,
             edited         =        FALSE,
             allocated      =        FALSE,
             item           =        FALSE;

   hword     what_to_get    =            1;



   s_init( &comp, "COMPOSE" );
   s_init( &sp,   "SP"      );
   s_init( &sigs, "RPAO" );
   s_init( &status, "OFC" );
   s_init( &reg_opt, "RO" );
   s_init( &scratchpad, "A * ABSTRACT $SP" );
   isv_get(&field_no, ISV_NO, 1);
   isv_get(&field_yes, ISV_YES, 1);
   s_init( &form, "A_CRQSMC");

   s_init( &ab_loc,      "AB_LOC"      );
   s_init( &num_parts,   "NUM_PARTS"   );
   s_init( &limit_mand,  "LIMIT_MAND"  );
   s_init( &signature,   "SIGNATURE"   );
   s_init( &view_before, "VIEW_BEFORE" );
   s_init( &by_per,      "BY_PER"      );
   s_init( &notf,        "NOTF"        );
   s_init( &sp_priv,     "SP_PRIV"     );
   s_init( &sequed,      "SEQUED"      );
   s_init( &stat,        "STAT"        );
   s_init( &to_do,       "TO_DO"       );
   s_init( &okto_cr,     "OKTO_CR"     );

   while ( !flag )
   {
      rc = t_form( form, S_NULL, NIL );

      if ((rc == USER_REQUESTED_ABANDON ) || ( rc == TM_FORM_NOT_FORMED)
         || (rc == USER_REQUESTED_QUIT) )
      {
         flag = TRUE;
      }
      else if ( ( rc == USER_REQUESTED_PROCESS)
              || ( rc == USER_REQUESTED_ENTER ) )

      {
         rc = 0;
#if 0
         /* once number of response levels allowed has been resolved */
         /* the #if will be removed                                  */
         if ( s_type( var_get(num_parts), 0, 0, 'N' ) != 0 )
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(num_parts),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, num_parts, 0.0, 0.0 );
            continue;
         }
         else
         {
            s_stosi( &levs, var_get(num_parts), 0, 0 );
            if ( (levs < 1) || (levs > 2) )
            {
               except( INVALID_CHOICE, USER_ERROR, var_get(num_parts),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
               f_curpos( form, NO_FLAGS, num_parts, 0.0, 0.0 );
               continue;
            }
         }
#endif
         if ( t_valyn( var_get(limit_mand) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(limit_mand),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, limit_mand, 0.0, 0.0 );
            continue;
         }
#if 0
         /* once different signature types are allowed will remove #if */
         s_copy(&sign, var_get(signature), 0, 0);
         s_upper(sign, 0, 0);
         if ( s_first ( sign, sigs, 0, 0) == 0 )
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(signature),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, signature, 0.0, 0.0 );
            continue;
         }
#endif
         if ( t_valyn( var_get(view_before) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(view_before),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, view_before, 0.0, 0.0 );
            continue;
         }
         if ( t_valyn( var_get(by_per) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(by_per),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, by_per, 0.0, 0.0 );
            continue;
         }
         if ( (rc_notf = t_valyn( var_get(notf) )) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(notf),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, notf, 0.0, 0.0 );
            continue;
         }
         if ( t_valyn( var_get(sp_priv) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(sp_priv),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, sp_priv, 0.0, 0.0 );
            continue;
         }
#if 0
         /* once sequencing of activities is implemented #if willbe out*/
         if ( s_comp( field_no, var_get(sequed), 0, 0, 'F', &xc ) != 0)
         {
            rc = obj_item(var_get(sequed), what_to_get, &oid2, NIL,
                                         &bad_items, &num_bad, ANY_OBJ);
            if (rc != 0)
            {
               except(ACCESS_DENIED_FOR_OBJECT, USER_ERROR,
                        var_get(sequed), S_NULL, S_NULL, S_NULL, S_NULL);
               f_curpos( form, NO_FLAGS, sequed, 0.0, 0.0 );
               continue;
            }
            else
            {
               obj_upd( act_ocb, "ACTIVITY.SEQ_OID", &oid2 );
            }
         }
#endif
         s_copy(&sta_tus, var_get(stat), 0, 0);
         s_upper(sta_tus, 0, 0);
         if ( s_first ( sta_tus, status, 0, 0 ) == 0 )
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(stat),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, stat, 0.0, 0.0 );
            continue;
         }
         s_copy(&options, var_get(to_do), 0, 0);
         s_upper(options, 0, 0);
         if (s_first( options, reg_opt, 0, 0) == 0)
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(to_do),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, to_do, 0.0, 0.0 );
            continue;
         }
         if ( s_comp( var_get(ab_loc), comp, 0, 0, 'F', &xc) != 0 )
         {
            if ( s_comp( var_get(ab_loc), sp, 0, 0, 'F', &xc) != 0 )
            {
#if 0
               /* once items are resolved #if will be taken out    */
               rc = obj_item(var_get(ab_loc),what_to_get,&oid1, NIL,
                                         &bad_items, &num_bad, ANY_OBJ);
               if (rc != 0)
               {
                  except(INPUT_EXCEPTION, USER_ERROR,var_get(ab_loc),
                                     S_NULL, S_NULL, S_NULL, S_NULL);
                  f_curpos( form, NO_FLAGS, ab_loc, 0.0, 0.0 );
                  item = FALSE;
                  continue;
               }
               else
               {
                  item = TRUE;
                  edited = FALSE;
               }
#endif
               /* once items are implemented will take out following */
               if (edited)
               {
                  f_setfld(form, ab_loc, sp);
               }
               else
               {
                  f_setfld(form, ab_loc, comp);
               }
               continue;
            }
         }
         if ( t_valyn( var_get(okto_cr) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(okto_cr),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form, NO_FLAGS, okto_cr, 0.0, 0.0 );
            continue;
         }
         else   /* all inputs are ok */
         {
            if (!allocated)
            {
               act_ocb = obj_all( ACT_OBJ, LOCAL_OBJECT, NIL );
               os_memc( &( act_ocb->oid ),  act_oid, sizeof(OID));
               allocated = TRUE;
            }
            if ( s_comp( var_get(ab_loc), comp, 0, 0, 'F', &xc) == 0 )
            {
               rc_spad = t_spad( scratchpad, &result );
               edited = TRUE;
               item = FALSE;
               if (t_valyn(var_get(okto_cr)) == 0 )
               {
                  f_setfld(form, okto_cr, field_yes);
                  f_setfld(form, ab_loc, sp);
                  continue;
               }
            }
         }
         rc = a_slnot( act_ocb, rc_notf, QUESTION);
         f_setfld(form, notf, field_no);
         if ( (rc == 0) && (t_valyn(var_get(okto_cr)) == 1 ) )
         {
            if (edited)
            {
               sp_ocb = obj_opn( (OID *)result, OBJ_READ | OS_FILE );
               if ( atch_ocb->type == MAIL_OBJ )
               {
                  ocd_new( &atch_ocd, "MESSAGE.CONTENTS" );
               }
               else if ( atch_ocb->type == COMMENT_OBJ )
               {
                  ocd_new( &atch_ocd, "COMMENT.CONTENTS" );
               }
               ocd_bse( atch_ocb, atch_ocd, sp_ocb );
               ocd_drp(&atch_ocd);
               obj_set_flag( sp_ocb, OBJ_DELETE);/* may not have to do */
               obj_cls( sp_ocb );                /* this since READ opn*/
            }
            else if (item)
            {
               /* link item */
            }
            else
            {
               f_rstfld(form, NO_FLAGS, okto_cr);
               f_rstfld(form, NO_FLAGS, ab_loc);
               continue;
            }
            a_adact( act_ocb, QUESTION );
            flag = TRUE;
            t_delsp( result );
         }
         else if (rc == USER_REQUESTED_ABANDON)
         {
            flag = TRUE;
            if ( OCB_POINTER_OK(act_ocb) )
            {
               obj_set_flag(act_ocb, OBJ_DELETE);
            }
         }
         else
         {
            if ( s_comp( var_get(ab_loc), sp, 0, 0, 'F', &xc) == 0 )
            {
               if ( (edited) || (item) )
               {
                  f_setfld(form, okto_cr, field_yes);
               }
               else
               {
                  f_rstfld(form, NO_FLAGS, ab_loc);
               }
            }
            continue;
         }
      }
   }        /* end of while loop */

   f_close(form, NO_FLAGS);
   if ( ( allocated ) && ( OCB_POINTER_OK ( act_ocb ) ) )
   {
      obj_cls(act_ocb);
   }
   var_clr(ab_loc);
   var_clr(notf);
   var_clr(limit_mand);
   var_clr(view_before);
   var_clr(num_parts);
   var_clr(signature);
   var_clr(by_per);
   var_clr(sp_priv);
   var_clr(sequed);
   var_clr(stat);
   var_clr(to_do);
   var_clr(okto_cr);


   s_drop(  &by_per   );
   s_drop(  &sp_priv  );
   s_drop(  &sequed   );
   s_drop(  &to_do    );
   s_drop(  &okto_cr  );
   s_drop(  &options  );
   s_drop(  &reg_opt  );
   s_drop(  &stat     );
   s_drop(  &status   );
   s_drop(  &sta_tus  );
   s_drop( &form );
   s_drop( &comp );
   s_drop( &sigs );
   s_drop( &sign );

   s_drop( &ab_loc );
   s_drop( &num_parts);
   s_drop( &limit_mand );
   s_drop( &view_before );
   s_drop( &notf );

   s_drop( &signature );
   s_drop( &scratchpad );
   s_drop( &result );
   s_drop( &sp );

   return(rc);
}
