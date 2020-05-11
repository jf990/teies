/************************************************************************
    <DOC>
    <NAME>         a_crsl(OCB *atch_ocb, OID *act_oid)
    <LEVEL>        Activity
    <DATE>         Feb 12, 1988      July 21, 1989
    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar
    <OUTPUT>       Sint.
    <PURPOSE>      To create the selection activity.
    <CALLS>        a_crrd(), a_crqs(), a_crsl(), obj_opn(), obj_upd(),
                   stosi(), s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), os_memn(), t_form(), isv_match(),
                   var_clr(), except().
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
#include "bse.h"
#include "activity.h"

sint a_crsl( atch_ocb, act_oid )

OCB   *atch_ocb;
OID   *act_oid;

{

   OCB     *act_ocb    =        NIL,
           *part_ocb   =        NIL,
           *p_ocb      =        NIL,
           *tbl_ocb    =        NIL,
           *sp_ocb     =        NIL;

   OCD     *atch_ocd   =        NIL,
           *tbl_ocd    =        NIL,
           *act_ocd    =        NIL;

   OID     *part_oid   =        NIL,
           oid1, oid2, oid3, oid4  ;

   boolean flag        =      FALSE,
           done        =      FALSE,
           allocated   =      FALSE,
           edited1     =      FALSE,
           edited2     =      FALSE,
           edited3     =      FALSE,
           item1       =      FALSE,
           item2       =      FALSE,
           item3       =      FALSE;

   sint    rc          =          0,
           rc_spad     =          0,
           xc          =          0,
           i           =          0,
           n_parts     =          0,
           rc_notf     =          0,
           bse_value   =          0,
          *line_num    =        NIL;

   tint    count       =          0,
           tbl_key     =       9999;

   hword   what_to_get = OBJ_NEED_ONE_OID,
           act_type    =   ISV_SELECTIO;

   string  form1       =     S_NULL,
           form2       =     S_NULL,
           x_str       =     S_NULL,
           tbl_line    =     S_NULL,
           line        =     S_NULL,
           blanks      =     S_NULL,
           para        =     S_NULL,
           who         =     S_NULL,
           result_oid  =     S_NULL,
           scratchpad  =     S_NULL,
           sec_sp      =     S_NULL,
           comp        =     S_NULL,
           result      =     S_NULL;

   /* variables used in forms */

   string  ab_loc       =    S_NULL,
           yes          =    S_NULL,
           no           =    S_NULL,
           sp           =    S_NULL,
           title        =    S_NULL,
           keys         =    S_NULL,
           sec_tab      =    S_NULL,
           cr_sec       =    S_NULL,
           limit_mand   =    S_NULL,
           signature    =    S_NULL,
           sigs         =    S_NULL,
           sign         =    S_NULL,
           sequed       =    S_NULL,
           stat         =    S_NULL,
           status       =    S_NULL,
           sta_tus      =    S_NULL,
           ok_toadd     =    S_NULL,
           num_sec      =    S_NULL,
           notf         =    S_NULL,
           options      =    S_NULL,
           reg_opt      =    S_NULL,
           to_do        =    S_NULL,
           sp_priv      =    S_NULL,
           sec_no       =    S_NULL,
           sec_loc      =    S_NULL,
           num_parts    =    S_NULL,
           okto_cr      =    S_NULL,
           another      =    S_NULL;


   s_init( &comp,       "COMPOSE"      );

   s_init( &ab_loc,     "AB_LOC"       );
   s_init( &cr_sec,     "CR_SEC"      );
   s_init( &sec_tab,    "SEC_TAB"      );
   s_init( &limit_mand,  "LIMIT_MAND"  );
   s_init( &signature,   "SIGNATURE"   );
   s_init( &num_sec,    "NUM_SEC"    );
   s_init( &stat,       "STAT"         );
   s_init( &sequed,     "SEQUED"      );
   s_init( &notf,       "NOTF"         );
   s_init( &sp_priv,    "SP_PRIV"      );
   s_init( &ok_toadd,   "OK_TOADD"    );
   s_init( &to_do,      "TO_DO"        );
   s_init( &another,    "ANOTHER"      );
   s_init( &sec_no,     "SEC_NO"       );
   s_init( &sec_loc,    "SEC_LOC"      );
   s_init( &title,      "TITLE"        );
   s_init( &keys,       "KEYS"        );
   s_init( &num_parts,  "NUM_PARTS"    );
   s_init( &okto_cr,    "OKTO_CR"    );
   s_init( &form1,      "A_CRSLMC"     );
   s_init( &blanks,     "  "           );
   s_init( &para,       "<p>"          );
   s_init( &who,        "NOT TAKEN"    );
   s_init( &sp,         "SP"     );
   s_init( &sigs, "RPAO" );
   s_init( &status, "OFC" );
   s_init( &reg_opt, "RO" );
   s_init( &scratchpad, "A * ABSTRACT $SP");
   s_init( &sec_sp, "A * SELECTION $SP");

   isv_get( &no, ISV_NO, 1 );
   isv_get( &yes, ISV_YES, 1 );

   while ( !flag )
   {
      rc = t_form( form1, S_NULL, NIL );

      if ((rc == USER_REQUESTED_ABANDON) || (rc == TM_FORM_NOT_FORMED)
         || (rc == USER_REQUESTED_QUIT) )
      {
         flag = TRUE;
      }
      else if ( ( rc == USER_REQUESTED_PROCESS )
              || ( rc == USER_REQUESTED_ENTER ) )

      {
         rc = 0;
         if ( t_valyn( var_get(cr_sec) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(cr_sec),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, cr_sec, 0.0, 0.0 );
            continue;
         }
         if ( s_type( var_get(limit_mand ), 0, 0, 'N' ) != 0 )
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(limit_mand),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, limit_mand, 0.0, 0.0 );
            continue;
         }
#if 0
         /* once diffrent types of signatures are allowed #if will be  */
         /* taken out                                                  */
         s_copy(&sign, var_get(signature), 0, 0);
         s_upper(sign, 0, 0);
         if ( s_first ( sign, sigs, 0, 0) == 0 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(signature),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, signature, 0.0, 0.0 );
            continue;
         }
#endif
         if ( (rc_notf = t_valyn( var_get(notf))) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(notf),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, notf, 0.0, 0.0 );
            continue;
         }
         if ( t_valyn( var_get(sp_priv) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(sp_priv),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, sp_priv, 0.0, 0.0 );
            continue;
         }
#if 0
         /* once sequencing of activities is implemented #if will be   */
         /* taken out                                                  */
         if ( s_comp( no, var_get(sequed), 1, 1, 'F', &xc ) != 0)
         {
            rc = obj_item(var_get(sequed), what_to_get, &oid4, NIL);
            if (rc != 0)
            {
               except( ACCESS_DENIED_FOR_OBJECT, USER_ERROR,
                        var_get(sequed), S_NULL, S_NULL, S_NULL, S_NULL);
               f_curpos( form1, NO_FLAGS, sequed, 0.0, 0.0 );
               continue;
            }
         }
#endif
         s_copy(&sta_tus, var_get(stat), 0, 0);
         s_upper(sta_tus, 0, 0);
         if ( s_first ( sta_tus, status, 0, 0 ) == 0 )
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(stat),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, stat, 0.0, 0.0 );
            continue;
         }
         s_copy(&options, var_get(to_do), 0, 0);
         s_upper(options, 0, 0);
         if (s_first( options, reg_opt, 0, 0) == 0)
         {
            except( INVALID_CHOICE, USER_ERROR, var_get(to_do),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, to_do, 0.0, 0.0 );
            continue;
         }

         if ( s_comp( var_get(ab_loc), comp, 0, 0, 'F', &xc) != 0 )
         {
            if ( s_comp( var_get(ab_loc), sp, 0, 0, 'F', &xc) != 0 )
            {
#if 0
               /* once items are implemented the #if will be removed */
               rc = obj_item(var_get(ab_loc),what_to_get,&oid1,NIL);
               if (rc != 0)
               {
                  except(ACCESS_DENIED_FOR_OBJECT, USER_ERROR,
                        var_get(ab_loc), S_NULL, S_NULL, S_NULL, S_NULL);
                  f_curpos( form1, NO_FLAGS, ab_loc, 0.0, 0.0 );
                  item1 = FALSE;
                  continue;
               }
               else
               {
                  item1 = TRUE;
                  edited1 = FALSE;
               }
#endif
               if (edited1)
               {
                  f_setfld(form1, ab_loc, sp);
               }
               else
               {
                  f_setfld(form1, ab_loc, comp);
               }
               continue;
            }

         }

#if 0
         /* once we allow items the #if will be removed  */
         if ( s_comp( var_get(sec_tab), comp, 0, 0, 'M', &xc) != 0 )
         {
            if ( s_comp( var_get(sec_tab), sp, 0, 0, 'F', &xc) != 0 )
            {
               rc = obj_item(var_get(sec_tab),what_to_get,&oid2,NIL);
               if (rc != 0)
               {
                  except(ACCESS_DENIED_FOR_OBJECT, USER_ERROR,
                       var_get(sec_tab), S_NULL, S_NULL, S_NULL, S_NULL);
                  f_curpos( form1, NO_FLAGS, sec_tab, 0.0, 0.0 );
                  item2 = FALSE;
                  continue;
               }
               else
               {
                  item2 = TRUE;
                  edited2 = FALSE;
               }
            }
         }
#endif
         if ( t_valyn( var_get(okto_cr) ) == -1 )
         {
            except( INPUT_EXCEPTION, USER_ERROR, var_get(okto_cr),
                                         S_NULL, S_NULL, S_NULL, S_NULL);
            f_curpos( form1, NO_FLAGS, okto_cr, 0.0, 0.0 );
            continue;
         }
         else   /* all inputs are ok */
         {
            if (!allocated)
            {
               act_ocb = obj_all( ACT_OBJ, LOCAL_OBJECT, NIL );
               os_memc( &( act_ocb->oid ),  act_oid, sizeof(OID));

               tbl_ocb = obj_all( CONFCOM_OBJ, LOCAL_OBJECT, NIL );
               ocd_new(&tbl_ocd, "COMMENT.ACTIVITY");
               obj_rep(tbl_ocb,  tbl_ocd, &(act_ocb->oid), SEG_UPDATE );
               s_init( &(tbl_ocd->name), "COMMENT.AUTHOR" );
               obj_rep(tbl_ocb, tbl_ocd, TM_master->ucb->name, SEG_UPDATE);
               s_init( &(tbl_ocd->name), "%LINKS.OWNER" );
               obj_rep(tbl_ocb, tbl_ocd, &(act_ocb->oid), SEG_UPDATE);

               ocd_new( &act_ocd, "ACTIVITY.PART_LIST.LOCATION" );
               s_alloc(&(act_ocd->key),4);
               s_smp_ul(act_ocd->key) = 4;
               os_memc( &tbl_key, s_buffer( act_ocd->key ), 4 );
               obj_rep( act_ocb, act_ocd, &(tbl_ocb->oid), SEG_UPDATE );
               ocd_drp(&act_ocd);
               ocd_drp(&tbl_ocd);
               allocated = TRUE;
            }
            /* composing the abstract */

            if ( s_comp( var_get(ab_loc), comp, 0, 0, 'F', &xc) == 0 )
            {
               rc_spad = t_spad( scratchpad, &result );
               edited1 = TRUE;
               item1 = FALSE;
               f_setfld(form1, ab_loc, sp);
            }
            if ( t_valyn( var_get(cr_sec) ) == 1 )
            {
               /* abstract created go now to create the sections      */

               obj_find(act_ocb, "ACTIVITY.PART_COUNT", &i);
               s_sitos( &x_str, count );
               var_set(sec_no, x_str);
               s_init( &form2, "A_CRSLCM" );
               f_curpos( form2, NO_FLAGS, title, 0.0, 0.0 );
               done = FALSE;
               while ( !done )  /* still more sections need info */
               {
                  rc = t_form( form2, S_NULL, NIL );

                  if ( rc == USER_REQUESTED_QUIT )
                  {
                     done = TRUE;
                     continue;
                  }

                  else if ( ( rc == USER_REQUESTED_ABANDON )
                          || ( rc == TM_FORM_NOT_FORMED ) )
                  {
                     done = TRUE;
                     flag = TRUE;
                     continue;
                  }

                  else if ( (rc == USER_REQUESTED_PROCESS )
                    || (rc == USER_REQUESTED_ENTER) )
                  {
                     rc = 0;
                     if ( t_valyn(var_get(another) ) == -1 )
                     {
                        except(INPUT_EXCEPTION,USER_ERROR,
                           var_get(another),S_NULL,S_NULL,S_NULL,S_NULL);
                        continue;
                     }

                     if (s_comp(var_get(sec_loc),comp,0,0,'F',&xc) != 0)
                     {
                        if (s_comp(var_get(sec_loc),sp,0,0,'F',&xc)!=0)
                        {
#if 0
                           /* once items are implemented #if will be   */
                           /* taken out                                */
                           rc = obj_item(var_get(sec_loc),what_to_get,
                                                              &oid3,NIL);
                           if (rc != 0)
                           {
                              except(ACCESS_DENIED_FOR_OBJECT,USER_ERROR,
                                     var_get(sec_loc), S_NULL, S_NULL,
                                                         S_NULL, S_NULL);
                              f_curpos(form2,NO_FLAGS,sec_loc,0.0,0.0);
                              item3 = FALSE;
                              continue;
                           }
                           else
                           {
                              item3 = TRUE;
                              edited3 = FALSE;
                           }
#endif
                           if (edited3)
                           {
                              f_setfld(form2, sec_loc, sp);
                           }
                           else
                           {
                              f_setfld(form2, sec_loc, comp);
                           }
                           continue;
                        }
                     }
                     if ( t_valyn(var_get(ok_toadd)) == -1 )
                     {
                        except(INPUT_EXCEPTION,USER_ERROR,
                          var_get(ok_toadd),S_NULL,S_NULL,S_NULL,S_NULL);
                        continue;
                     }
                     else     /* all inputs ok */
                     {
                        if ( s_comp ( var_get(sec_loc), comp,
                                                 0, 0, 'F', &xc) == 0 )
                        {

                           rc_spad = t_spad( sec_sp, &result_oid );
                           edited3 = TRUE;
                           item3 = FALSE;
                           if ((t_valyn (var_get (ok_toadd) )) == 0 )
                           {
                              f_setfld(form2, sec_loc, sp);
                              f_setfld(form2, ok_toadd, yes);
                              f_curpos(form2, NO_FLAGS,ok_toadd,0.0,0.0);
                              continue;
                           }
                        }
                     }
                     if (t_valyn (var_get (ok_toadd)) == 1 )
                     {
                        if (edited3)
                        {
                           count++;
                           a_crpt(result_oid,act_ocb,count,
                                                       &p_ocb,act_type);
                           obj_find(p_ocb, "COMMENT.KEYS", &tbl_line);
                           obj_cls(p_ocb);
                           s_sitos( &line, count );
                           s_pad(&line, line, 3, ' ', 'R');
                           s_cat(&line, line, blanks, 0, 0);
                           s_cat(&line, line, tbl_line, 0, 0);
                           s_pad(&line, line, 32, ' ', 'R');
                           s_cat(&line, line, who, 0, 0);
                           s_cat(&line, line, para, 0, 0);
                           ocd_new(&tbl_ocd, "COMMENT.CONTENTS.TEXT");
                           s_alloc(&(tbl_ocd->key),4);
                           s_smp_ul(tbl_ocd->key) = 4;
                           line_num = (sint *)s_buffer(tbl_ocd->key);
                           *(line_num) = count;
                           bse_value = B_RCONT | B_PRINTABLE;
                           s_init(&(tbl_ocd->name),
                                                "COMMENT.CONTENTS.TEXT");
                           obj_rep(tbl_ocb, tbl_ocd,line,SEG_UPDATE);
                           s_init(&(tbl_ocd->name),
                                                "COMMENT.CONTENTS.BSE");
                           obj_rep(tbl_ocb,tbl_ocd,&bse_value,
                                                             SEG_UPDATE);
                           ocd_drp(&tbl_ocd);
                           t_delsp(sec_sp);
                        }
                        else if (item3)
                        {
                           /* need to link item */
                        }
                        else
                        {
                           f_rstfld(form2, NO_FLAGS, sec_loc);
                           f_rstfld(form2, NO_FLAGS, ok_toadd);
                           continue;
                        }

                        f_rstfld(form2, NO_FLAGS, title);
                        f_rstfld(form2, NO_FLAGS, keys);
                        edited3 = FALSE;
                        item3 = FALSE;
                        s_sitos( &x_str, count );
                        f_setfld( form2, sec_no, x_str);
                        /* this is for count on form1 */
                        f_setfld( form1, num_sec, x_str);

                        f_setfld( form2, sec_loc, comp);
                        f_setfld( form2, ok_toadd, no);
                        f_curpos( form2, NO_FLAGS, another, 0.0, 0.0 );
                     }
                     if ( s_comp(var_get(sec_loc),sp,0,0,'F',&xc) == 0)
                     {
                        if ( (edited3) || (item3) )
                        {
                           f_setfld(form2, ok_toadd, yes);
                        }
                        else
                        {
                           f_rstfld(form2, NO_FLAGS, sec_loc);
                           f_rstfld(form2, NO_FLAGS, ok_toadd);
                        }
                     }
                     if ( t_valyn(var_get(another) ) == TRUE )
                     {
                        done = TRUE;
                        var_clr(sec_loc);
                        var_clr(title);
                        var_clr(keys);
                        var_clr(ok_toadd);
                        var_clr(another);
                        f_setfld( form1, cr_sec, no);
                        rc = 0;
                        continue;
                     }
                  }
                  else
                  {
                     continue;
                  }
               }   /* end of inner while-loop */

               f_close(form2, NO_FLAGS);
               if (rc == USER_REQUESTED_QUIT)
               {
                  rc = 0;
                  f_setfld( form1, cr_sec, no);
               }
               else if (rc == USER_REQUESTED_ABANDON)
               {
                  break;
                  /* NEED TO DO CLEANUP */
               }
               else
               {

                  if ( s_comp(var_get(ab_loc), sp, 0, 0, 'F', &xc) == 0 )
                  {
                     if ( (edited1) || (item1) )
                     {
                        f_setfld( form1, okto_cr, yes);
                        continue;
                     }
                     else
                     {
                        f_rstfld(form1, NO_FLAGS, ab_loc);
                        f_rstfld(form1, NO_FLAGS, okto_cr);
                        continue;
                     }
                  }

               }
            }
         }
         rc = a_slnot( act_ocb, rc_notf, SELECTION);
         f_setfld(form1, notf, no);
         if ( (rc == 0) && (t_valyn(var_get(okto_cr)) == 1 ) )
         {
            if (edited1)
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
               t_delsp( result );
            }
            else if (item1)
            {
               /* link item */
            }
            else
            {
               f_rstfld(form1, NO_FLAGS, okto_cr);
               f_rstfld(form1, NO_FLAGS, ab_loc);
               continue;
            }

            if ( !OID_IS_NULL(&oid4) )
            {
               obj_upd( act_ocb, "ACTIVITY.SEQ_OID", &oid4 );
            }
            a_adact( act_ocb, SELECTION );

            obj_cls(tbl_ocb);


            flag = TRUE;
         }
         else if (rc == USER_REQUESTED_ABANDON)
         {
            flag = TRUE;
         }
         else
         {
            if ( s_comp( var_get(ab_loc), sp, 0, 0, 'F', &xc) == 0 )
            {
               if ( (edited1) || (item1) )
               {
                  f_setfld(form1, okto_cr, yes);
               }
               else
               {
                  f_rstfld(form1, NO_FLAGS, ab_loc);
               }
            }
            continue;
         }
      }
      else
      {
         continue;
      }
   }        /* end of while loop */

   f_close(form1, NO_FLAGS);

   if ( (rc == USER_REQUESTED_QUIT) || (rc == USER_REQUESTED_ABANDON) )
   {
      if ( OCB_POINTER_OK(act_ocb) )
      {
         obj_find(act_ocb, "ACTIVITY.PART_COUNT", &n_parts);
         ocd_new( &act_ocd, "ACTIVITY.PART_LIST.LOCATION" );
         s_alloc(&(act_ocd->key),4);
         s_smp_ul(act_ocd->key) = 4;

         part_oid = ( OID * )os_mema( sizeof (OID) );
         for ( i=1; i<= n_parts; i++)
         {
            os_memc( &i, s_buffer( act_ocd->key ), 4 );

            obj_get( act_ocb, act_ocd, part_oid );
            part_ocb = obj_opn( part_oid, OBJ_UPDATE );
            obj_set_flag(part_ocb, OBJ_DELETE);
            obj_cls(part_ocb);
         }
         ocd_drp(&act_ocd);
         os_memd(part_oid);
         obj_set_flag(act_ocb, OBJ_DELETE);
      }
      if ( OCB_POINTER_OK (tbl_ocb) )
      {
         obj_set_flag(tbl_ocb, OBJ_DELETE);
         obj_cls(tbl_ocb);
      }
   }
   if ( ( allocated ) && ( OCB_POINTER_OK (act_ocb) ) )
   {
      obj_cls(act_ocb);
   }

   /* Variables Used in Froms */

   var_clr(ab_loc);
   var_clr(cr_sec);
   var_clr(sec_tab);
   var_clr(num_sec);
   var_clr(stat  );
   var_clr(to_do );
   var_clr(sequed);
   var_clr(notf  );
   var_clr(sp_priv);
   var_clr(okto_cr);
   var_clr(signature);
   var_clr(limit_mand);

   var_clr(sec_no );
   var_clr(sec_loc);
   var_clr(title  );
   var_clr(keys  );
   var_clr(ok_toadd);
   var_clr(another);

   s_drop( &ab_loc );
   s_drop( &cr_sec );
   s_drop( &sec_tab );
   s_drop( &num_sec );
   s_drop( &stat );
   s_drop( &status );
   s_drop( &sta_tus );
   s_drop( &to_do );
   s_drop( &sequed );
   s_drop( &notf );
   s_drop( &options );
   s_drop( &sp_priv );
   s_drop( &okto_cr );
   s_drop( &sign );
   s_drop( &signature );
   s_drop( &limit_mand);

   s_drop( &sec_no );
   s_drop( &sec_loc );
   s_drop( &title );
   s_drop( &keys );
   s_drop( &ok_toadd );
   s_drop(&another);
   s_drop( &num_parts );
   s_drop( &result_oid);

   s_drop( &x_str );
   s_drop( &form1);
   s_drop( &form2);
   s_drop( &tbl_line);
   s_drop( &line);
   s_drop( &blanks);
   s_drop( &who);
   s_drop( &para);
   s_drop( &sigs);
   s_drop( &comp );
   s_drop( &scratchpad );
   s_drop( &sec_sp );
   s_drop( &sp );
   s_drop( &result  );


   return(rc);
}
