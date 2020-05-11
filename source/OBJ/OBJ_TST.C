/*+-------------------------------------------------------------------+*/
/*|     :NAME.                 OBJ_TST.c                              |*/
/*|                                                                   |*/
/*|     :LEVEL.                OBJ                                    |*/
/*|                                                                   |*/
/*|     :AUTHOR.               John L. Foster, Ashish K. Mehta        |*/
/*|                                                                   |*/
/*|     :DATE.                 July 28, 1988                          |*/
/*|                                                                   |*/
/*|     :PARAMS.               -None-                                 |*/
/*|                                                                   |*/
/*|     :OUTPUT.        sint: 0  If user request 'Quit' without       |*/
/*|                              testing any routine.                 |*/
/*|                              OR                                   |*/
/*|                              All the TESTs   completed            |*/
/*|                              successfully.                        |*/
/*|                                                                   |*/
/*|                           -- Standard return code from OBJECT     |*/
/*|                              LEVEL routines.                      |*/
/*|                                                                   |*/
/*|     :GLOBALS.              - none -                               |*/
/*|                                                                   |*/
/*|     :PURPOSE.          Object level routine test bed.             |*/
/*|                        This  routine gives USER SCREEN  for       |*/
/*|                        selecting particular sub_level.            |*/
/*|                        Particular sub_level gives screen for      |*/
/*|                        selecting particular routine user wants    |*/
/*|                        to test.                                   |*/
/*|                        On each screen 'all' selection  will       |*/
/*|                        test all routines.                         |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|     :CALLS.     t_gtdate(),                                       |*/
/*|                 s_init  (), s_comp  (), s_stosi (),               |*/
/*|                 var_set (), var_clr (), f_rstfld(),               |*/
/*|                 f_setfld(), s_cat   (),                           |*/
/*|                                                                   |*/
/*|                 All Object level test routines.                   |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|     :ERRORS.                                                      |*/
/*|                                                                   |*/
/*|     :LIMITS.                                                      |*/
/*|                                                                   |*/
/*|     :MODS.      Nov_16_88 A.K.Mehta  Updated such that test_obj   |*/
/*|                                      calls obj_tst()              |*/
/*|                 Apr_18_88 A.K.Mehta  Added new obj level routines |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "obj.h"
#include "tm.h"
#include "form.h"
#include "obj_test.h"
#include "vars.h"


#define   ALL_OBJ_TEST         1      /* test all OBJ level test rtns  */
#define   OBJ_OBJ_TEST         2      /* select obj_obj level rtns only*/
#define   OBJ_ODT_TEST         3      /* select obj_odt level rtns only*/
#define   OBJ_OCB_TEST         4      /* select obj_ocb level rtns only*/
#define   OBJ_OCD_TEST         5      /* select obj_ocd level rtns only*/
#define   OBJ_OID_TEST         6      /* select obj_oid level rtns only*/
#define   OBJ_SOIDI_TEST       7      /* select obj_soidi lvl rtns only*/
#define   OBJ_PSI_TEST         8      /* select obj_psi level rtns only*/
#define   OBJ_LST_TEST         9      /* select obj_lst level rtns only*/


#define   OBJ_OBJ_RTNS        50      /* Total obj_obj level rtns      */
#define   OBJ_ODT_RTNS        13      /* Total obj_odt level rtns      */
#define   OBJ_OCB_RTNS         4      /* Total obj_ocb level rtns      */
#define   OBJ_OCD_RTNS        15      /* Total obj_ocd level rtns      */
#define   OBJ_OID_RTNS         4      /* Total obj_oid level rtns      */
#define   OBJ_SOI_RTNS         5      /* Total obj_soi level rtns      */
#define   OBJ_PSI_RTNS         3      /* Total obj_psi level rtns      */
#define   OBJ_LST_RTNS        16      /* Total obj_lst level rtns      */

#define   SUB_LEVEL_CHOICE     9      /* Total choices for test        */

sint obj_tst ()

{

     OCB         *ocb           =    NIL      ;
     sint        rc             =    0        ,
                 sub_level      =    0        ,
                 process_id     =    0        ,
                 sub_lvl_rtn    =    0        ;
     string      modifier       =    S_NULL   ,
                 form_id        =    S_NULL   ,
                 form_sub       =    S_NULL   ,
                 choice         =    S_NULL   ,
                 current_dt     =    S_NULL   ,
                 DATE           =    S_NULL   ,
                 message        =    S_NULL   ,
                 return_string  =    S_NULL   ,
                 abandon        =    S_NULL   ,
                 quit           =    S_NULL   ,
                 message1       =    S_NULL   ,
                 message2       =    S_NULL   ,
                 message3       =    S_NULL   ,
                 temp_message   =    S_NULL   ;





     #D_begin ( obj_tst, OBJ                                           );
     #D_say   ( " BEGIN OBJ_TEST "                                     );



     s_init   ( &message ,"MESSAGE"                                    );
     s_init   ( &DATE    ,"%TIMEON"                                    );
     s_init   ( &form_id ,"obj_test"                                   );
     s_init   ( &choice  ,"CHOICE"                                     );
     s_init   ( &quit    ,"QUIT"                                       );
     s_init   ( &abandon ,"ABANDON"                                    );
     s_init   ( &message1,"That choice is not in the menu. Try again"  );
     s_init   ( &message2," * * Test  Complete. No error Noticed  * * ");
     s_init   ( &message3,"* * * *  Test Complete . Error Found . "    );
     s_init   ( &temp_message," Press any key to continue. * * * *"    );

     /*+--------------------------------------------------------------+*/
     /*|    s_cat    is used because the string message3 was long.    |*/
     /*|    t_gtdate is used to get  current date and time.           |*/
     /*+--------------------------------------------------------------+*/

     s_cat    ( &message3,  message3,   temp_message,  0, 0          )             ;

     t_gtdate ( &current_dt,  NIL                                    );
     var_set  ( DATE,   current_dt                                   );


     /*+------------------------------------------------------------+*/
     /*|       call FIRST screen to select sub_level.               |*/
     /*|             - if user selects an invalid sublevel then     |*/
     /*|               gives message1 and then user can select      |*/
     /*|               valid sublevel.                              |*/
     /*+------------------------------------------------------------+*/

     while ( s_comp  (  abandon, return_string, 0, 0, 'M', &rc ) != 0  )

     {

        return_string  =  t_form (  form_id,   S_NULL,    NIL        );

        if (  return_string   ==   S_NULL                            ||
              t_ifquit ( return_string )                               )
        {

            #D_say('User Requested  QUIT'    );
            break;

        }

        else
        {

          /*+---------------------------------------------------------+*/
          /*|    Convert string returned by t_form into integer.      |*/
          /*|    Reset the choice field of form_id  form.             |*/
          /*+---------------------------------------------------------+*/


          rc = s_stosi( &sub_level,  var_get(choice), 1,  2            );
          #D_show  ( 'sublevel =%d', 'sub_level'                       );


          f_rstfld ( form_id, NO_FLAGS, choice                         );
          var_clr(choice);


          while (  sub_level > SUB_LEVEL_CHOICE   ||    sub_level < 1  )

          {
              if ( sub_level != 0 )
              {

                  f_setfld ( form_id,   message,   message1 );
                  t_alarm ( process_id );

              }

              return_string = t_form( form_id, S_NULL, NIL );

              if ( return_string == S_NULL                   ||
                   t_ifquit ( return_string )                 )
              {

                  #D_say ('User Requested  QUIT');
                  s_init(&return_string, "ABANDON" );
                  break;

              }

              else
              {

                  rc = s_stosi( &sub_level, var_get(choice), 1, 2 );
                  #D_show  ('sublevel =%d', 'sub_level'           );

                  f_rstfld ( form_id, NO_FLAGS,  message          );
                  var_clr( message );

                  f_rstfld ( form_id, NO_FLAGS, choice            );
                  var_clr(choice);

              }
          }



        /*+---------------------------------------------------------+*/
        /*|    If user selects ALL_OBJ_TEST at the first screen then|*/
        /*|                call ALL the functions in the TEST SUITE.|*/
        /*|    Else call second screen ( SUB_LEVEL ) selected       |*/
        /*|                by user.                                 |*/
        /*|         -- At each sub_level user can select  ALL       |*/
        /*|            to call all the functions at that sub_level. |*/
        /*|         -- AT each sub_level if user selects an invalid |*/
        /*|            function then print error message and then   |*/
        /*|            user can select valid function.              |*/
        /*+---------------------------------------------------------+*/


          switch(sub_level)
          {

            case  ALL_OBJ_TEST   :

                        rc   =   tobjalk   ();
                        rc   =   tobjall   ();
                        rc   =   tobjapp   ();
                        rc   =   tobjbeg   ();
                        rc   =   tobjcls   ();
                        rc   =   tobjcmod  ();
                        rc   =   tobjcopy  ();
                        rc   =   tobjcoun  ();
                        rc   =   tobjdec   ();
                        rc   =   tobjdel   ();
                        rc   =   tobjdlst  ();
                        rc   =   tobjdms   ();
                        rc   =   tobjend   ();
                        rc   =   tobjenc   ();
                        rc   =   tobjexpo  ();
                        rc   =   tobjfind  ();
                        rc   =   tobjfmt   ();
                        rc   =   tobjfocd  ();
                        rc   =   tobjgaol  ();
                        rc   =   tobjget   ();
                        rc   =   tobjglk   ();
                        rc   =   tobjgrec  ();
                        rc   =   tobjimpo  ();
                        rc   =   tobjini   ();
                        rc   =   tobjint   ();
                        rc   =   tobjina   ();
                        rc   =   tobjitem  ();
                        rc   =   tobjkey   ();
                        rc   =   tobjlink  ();
                        rc   =   tobjlnk   ();
                        rc   =   tobjlns   ();
                        rc   =   tobjmove  ();
                        rc   =   tobjname  ();
                        rc   =   tobjnext  ();
                        rc   =   tobjopn   ();
                        rc   =   tobjown   ();
                        rc   =   tobjpack  ();
                        rc   =   tobjreco  ();
                        rc   =   tobjrep   ();
                        rc   =   tobjrlk   ();
                        rc   =   tobjrmv   ();
                        rc   =   tobjsdel  ();
                        rc   =   tobjsync  ();
                        rc   =   tobjsysn  ();
                        rc   =   tobjtini  ();
                        rc   =   tobjtype  ();
                        rc   =   tobjunpa  ();
                        rc   =   tobjupd   ();
                        rc   =   tobjunli  ();
                        rc   =   todtadd   ();
                        rc   =   todtdini  ();
                        rc   =   todtdisp  ();
                        rc   =   todtfind  ();
                        rc   =   todtinit  ();
                        rc   =   todtlen   ();
                        rc   =   todtload  ();
                        rc   =   todtlink  ();
                        rc   =   todtmake  ();
                        rc   =   todtsave  ();
                        rc   =   todtsgln  ();
                        rc   =   todtptr   ();
                        rc   =   tocball   ();
                        rc   =   tocbdrp   ();
                        rc   =   tocbinta  ();
                        rc   =   tocdbse   ();
                        rc   =   tocdctos  ();
                        rc   =   tocddbp   ();
                        rc   =   tocddcd   ();
                        rc   =   tocddrp   ();
                        rc   =   tocdini   ();
                        rc   =   tocdkey   ();
                        rc   =   tocdlen   ();
                        rc   =   tocdnew   ();
                        rc   =   tocdnext  ();
                        rc   =   tocdprev  ();
                        rc   =   tocdroot  ();
                        rc   =   tocdsgln  ();
                        rc   =   tocdstoc  ();
                        rc   =   toidnew   ();
                        rc   =   toidkey   ();
                        rc   =   toidcmp   ();
                        rc   =   tsoidiad  ();
                        rc   =   tsoiddl   ();
                        rc   =   tsoiddu   ();
                        rc   =   tsoidigt  ();
                        rc   =   tpsiadd   ();
                        rc   =   tpsidump  ();
                        rc   =   tablist   ();
                        rc   =   tcrlist   ();
                        rc   =   tdolist   ();
                        rc   =   texlist   ();
                        rc   =   tfnlist   ();
                        rc   =   tgtlist   ();
                        rc   =   tjnlist   ();
                        rc   =   tlistfun  ();
                        rc   =   tlklist   ();
                        rc   =   tmdlist   ();
                        rc   =   torlist   ();
                        rc   =   trclist   ();
                        rc   =   trmlist   ();
                        rc   =   trvlist   ();
                        rc   =   tsclist   ();

                       /*+-----------------------------------------+*/
                       /*|    If rc = 0 then no errors.            |*/
                       /*|    else has occurred.                   |*/
                       /*+-----------------------------------------+*/

                       if ( rc != 0 )
                       {

                         f_setfld ( form_id , message, message3);

                       }

                       else
                       {

                         f_setfld ( form_id , message, message2);

                       }

                       t_alarm( process_id );
                       return_string = t_form ( form_id, S_NULL, NIL);

                       f_rstfld (   form_id,  NO_FLAGS,  message      );
                       var_clr( message );

                       f_rstfld (  form_id,  NO_FLAGS,  choice        );
                       var_clr(choice);
                       break;

          case  OBJ_OBJ_TEST  :

                       s_init ( &form_sub, "obj_obj"    );
                       return_string = t_form (form_sub, S_NULL, NIL);

                       while ( return_string != S_NULL           &&
                               ! (t_ifquit ( return_string ))      )
                       {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                         f_rstfld ( form_sub, NO_FLAGS, choice );
                         var_clr(choice);

                         /*+---------------------------------------+*/
                         /*| case when selected routine is not in  |*/
                         /*|         the menu.                     |*/
                         /*+---------------------------------------+*/

                         while (   sub_lvl_rtn > OBJ_OBJ_RTNS       ||
                                   sub_lvl_rtn < 1                    )
                         {

                           if ( sub_lvl_rtn != 0 )
                           {

                               f_setfld (form_sub, message, message1);
                               t_alarm ( process_id );

                           }


                           return_string = t_form(form_sub,S_NULL,NIL);

                           if ( return_string == S_NULL       ||
                                t_ifquit ( return_string )     )

                           {

                              break;

                           }
                           else
                           {

                              s_stosi(&sub_lvl_rtn,var_get(choice),1,2 );
                              #D_show ('sublevel =%d', 'sub_lvl_rtn'   );

                              f_rstfld ( form_sub, NO_FLAGS, message   );
                              var_clr( message );

                              f_rstfld ( form_sub, NO_FLAGS, choice    );
                              var_clr(choice);

                           }
                         }

                      if ( return_string != S_NULL                &&
                           ! (t_ifquit ( return_string ))          )
                      {

                       switch ( sub_lvl_rtn )
                       {

                       case   1 :

                                rc   =   tobjalk   ();
                                rc   =   tobjall   ();
                                rc   =   tobjapp   ();
                                rc   =   tobjbeg   ();
                                rc   =   tobjcls   ();
                                rc   =   tobjcmod  ();
                                rc   =   tobjcopy  ();
                                rc   =   tobjcoun  ();
                                rc   =   tobjdec   ();
                                rc   =   tobjdel   ();
                                rc   =   tobjdlst  ();
                                rc   =   tobjdms   ();
                                rc   =   tobjend   ();
                                rc   =   tobjenc   ();
                                rc   =   tobjexpo  ();
                                rc   =   tobjfind  ();
                                rc   =   tobjfmt   ();
                                rc   =   tobjfocd  ();
                                rc   =   tobjgaol  ();
                                rc   =   tobjget   ();
                                rc   =   tobjglk   ();
                                rc   =   tobjgrec  ();
                                rc   =   tobjimpo  ();
                                rc   =   tobjini   ();
                                rc   =   tobjint   ();
                                rc   =   tobjina   ();
                                rc   =   tobjitem  ();
                                rc   =   tobjkey   ();
                                rc   =   tobjlink  ();
                                rc   =   tobjlnk   ();
                                rc   =   tobjlns   ();
                                rc   =   tobjmove  ();
                                rc   =   tobjname  ();
                                rc   =   tobjnext  ();
                                rc   =   tobjopn   ();
                                rc   =   tobjown   ();
                                rc   =   tobjpack  ();
                                rc   =   tobjreco  ();
                                rc   =   tobjrep   ();
                                rc   =   tobjrlk   ();
                                rc   =   tobjrmv   ();
                                rc   =   tobjsdel  ();
                                rc   =   tobjsync  ();
                                rc   =   tobjsysn  ();
                                rc   =   tobjtini  ();
                                rc   =   tobjtype  ();
                                rc   =   tobjunpa  ();
                                rc   =   tobjupd   ();
                                rc   =   tobjunli  ();
                                break;
                       case   2 :
                                rc = tobjalk ();
                                break;
                       case   3 :
                                rc = tobjall ();
                                break;
                       case   4 :
                                rc = tobjapp ();
                                break;
                       case   5 :
                                rc = tobjbeg ();
                                break;
                       case   6 :
                                rc = tobjcls ();
                                break;
                       case   7 :
                                rc = tobjcmod ();
                                break;
                       case   8 :
                                rc = tobjcopy();
                                break;
                       case   9 :
                                rc = tobjcoun();
                                break;
                       case  10 :
                                rc = tobjdec ();
                                break;
                       case  11 :
                                 rc = tobjdel ();
                                 break;
                       case  12 :
                                rc = tobjdlst();
                                break;
                       case  13 :
                                rc = tobjdms ();
                                break;
                       case  14 :
                                rc = tobjend ();
                                break;
                       case  15 :
                                rc = tobjenc ();
                                break;
                       case  16 :
                                rc = tobjexpo ();
                                break;
                       case  17 :
                                rc = tobjfind();
                                break;
                       case  18 :
                                rc = tobjfmt ();
                                break;
                       case  19 :
                                rc = tobjfocd();
                                break;
                       case  20 :
                                rc = tobjgaol ();
                                break;
                       case  21 :
                                rc = tobjget ();
                                break;
                       case  22 :
                                rc = tobjglk ();
                                break;
                       case  23 :
                                rc = tobjgrec();
                                break;
                       case  24 :
                                rc = tobjimpo ();
                                break;
                       case  25 :
                                rc = tobjini ();
                                break;
                       case  26 :
                                rc = tobjint  ();
                                break;
                       case  27 :
                                rc = tobjina ();
                                break;
                       case  28 :
                                rc = tobjitem ();
                                break;
                       case  29 :
                                rc = tobjkey ();
                                break;
                       case  30 :
                                rc = tobjlink ();
                                break;
                       case  31 :
                                rc = tobjlnk ();
                                break;
                       case  32 :
                                rc = tobjlns  ();
                                break;
                       case  33 :
                                rc = tobjmove ();
                                break;
                       case  34 :
                                rc = tobjname();
                                break;
                       case  35 :
                                rc = tobjnext();
                                break;
                       case  36 :
                                rc = tobjopn ();
                                break;
                       case  37 :
                                rc = tobjown ();
                                break;
                       case  38 :
                                rc = tobjpack();
                                break;
                       case  39 :
                                rc = tobjreco();
                                break;
                       case  40 :
                                rc = tobjrep ();
                                break;
                       case  41 :
                                rc = tobjrlk ();
                                break;
                       case  42 :
                                rc = tobjrmv ();
                                break;
                       case  43 :
                                rc = tobjsdel();
                                break;
                       case  44 :
                                rc = tobjsync();
                                break;
                       case  45 :
                                rc = tobjsysn ();
                                break;
                       case  46 :
                                rc = tobjtini ();
                                break;
                       case  47 :
                                rc = tobjtype();
                                break;
                       case  48 :
                                rc = tobjunli ();
                                break;
                       case  49 :
                                rc = tobjunpa ();
                                break;
                       case  50 :
                                rc = tobjupd  ();
                                break;

                       }
                       if ( rc != 0 )
                       {

                           f_setfld ( form_sub, message, message3);

                       }
                       else
                       {

                           f_setfld ( form_sub, message, message2);

                       }
                       t_alarm( process_id );
                       return_string = t_form( form_sub, S_NULL, NIL);

                       f_rstfld ( form_sub, NO_FLAGS, message);
                       var_clr( message );

                       f_rstfld ( form_sub, NO_FLAGS, choice);
                       var_clr(choice);
                       }
                      }
                      break;

          case  OBJ_ODT_TEST  :

                      s_init ( &form_sub, "obj_odt"    );
                      return_string = t_form (form_sub, S_NULL, NIL);

                      while ( return_string != S_NULL            &&
                              ! (t_ifquit ( return_string ))       )
                      {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2 );

                         f_rstfld ( form_sub, NO_FLAGS, choice        );
                         var_clr(choice);


                          while (  sub_lvl_rtn > OBJ_ODT_RTNS            ||
                                   sub_lvl_rtn < 1                        )
                          {

                            if ( sub_lvl_rtn != 0 )
                            {

                               f_setfld (form_sub, message, message1);
                               t_alarm ( process_id );

                            }


                            return_string = t_form(form_sub, S_NULL,NIL);

                            if ( return_string == S_NULL           ||
                                 t_ifquit ( return_string )         )
                            {

                               break;

                            }
                            else
                            {

                               s_stosi(&sub_lvl_rtn,var_get(choice),1,2);
                               #D_show ('sublevel =%d', 'sub_lvl_rtn');

                               f_rstfld ( form_sub, NO_FLAGS, message);
                               var_clr( message );

                               f_rstfld ( form_sub, NO_FLAGS, choice);
                               var_clr(choice);

                            }
                          }

                          if ( return_string != S_NULL         &&
                               ! (t_ifquit ( return_string ) )   )
                          {

                          switch ( sub_lvl_rtn )
                          {

                          case   1 :
                                   rc  = todtadd   ();
                                   rc  = todtdini  ();
                                   rc  = todtdisp  ();
                                   rc  = todtfind  ();
                                   rc  = todtinit  ();
                                   rc  = todtlen   ();
                                   rc  = todtload  ();
                                   rc  = todtlink  ();
                                   rc  = todtmake  ();
                                   rc  = todtsave  ();
                                   rc  = todtsgln  ();
                                   rc  = todtptr   ();
                                   break;
                          case   2 :
                                   rc = todtadd ();
                                   break;
                          case   3 :
                                   rc = todtdini();
                                   break;
                          case   4 :
                                   rc = todtdisp();
                                   break;
                          case   5 :
                                   rc = todtfind();
                                   break;
                          case   6 :
                                   rc = todtinit();
                                   break;
                          case   7 :
                                   rc = todtlen ();
                                   break;
                          case   8 :
                                   rc = todtload();
                                   break;
                          case   9 :
                                   rc = todtlink();
                                   break;
                          case  10 :
                                   rc = todtmake();
                                   break;
                          case  11 :
                                   rc = todtsave();
                                   break;
                          case  12 :
                                   rc = todtsgln();
                                   break;
                          case  13 :
                                   rc = todtptr ();
                                   break;
                         }
                         if ( rc != 0 )
                         {


                           f_setfld ( form_sub, message, message3);

                         }
                         else
                         {

                           f_setfld ( form_sub, message, message2);

                         }
                         t_alarm( process_id );
                         return_string = t_form(form_sub, S_NULL, NIL);

                         f_rstfld ( form_sub, NO_FLAGS, message      );
                         var_clr( message );

                         f_rstfld ( form_sub, NO_FLAGS, choice       );
                         var_clr(choice);
                        }
                      }
                      break;

          case  OBJ_OCB_TEST  :

                      s_init ( &form_sub, "obj_ocb"    );
                      return_string = t_form(form_sub, S_NULL, NIL);

                      while ( return_string != S_NULL           &&
                              ! ( t_ifquit ( return_string ) )    )
                      {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                         f_rstfld ( form_sub, NO_FLAGS, choice );
                         var_clr(choice);


                         while (  sub_lvl_rtn > OBJ_OCB_RTNS     ||
                                  sub_lvl_rtn < 1                 )
                         {

                            if ( sub_lvl_rtn != 0 )
                            {

                                f_setfld (form_sub, message, message1);
                                t_alarm ( process_id );

                            }

                            return_string = t_form(form_sub,S_NULL,NIL);

                            if ( return_string == S_NULL            ||
                                 t_ifquit ( return_string )           )
                            {

                               break;

                            }
                            else
                            {

                               s_stosi(&sub_lvl_rtn,var_get(choice),1,2);
                               #D_show ('sublevel =%d', 'sub_lvl_rtn');

                               f_rstfld ( form_sub, NO_FLAGS, message);
                               var_clr( message );

                               f_rstfld ( form_sub, NO_FLAGS, choice);
                               var_clr(choice);

                            }
                         }

                         if ( return_string != S_NULL          &&
                              ! ( t_ifquit ( return_string ))    )
                         {

                          switch ( sub_lvl_rtn )
                          {

                          case   1 :
                                   rc = tocball ();
                                   rc = tocbdrp ();
                                   rc = tocbinta();
                                   break;
                          case   2 :
                                   rc = tocball ();
                                   break;
                          case   3 :
                                   rc = tocbdrp ();
                                   break;
                          case   4 :
                                   rc = tocbinta();
                                   break;
                           }
                         if ( rc != 0 )
                         {

                           f_setfld ( form_sub, message, message3);

                         }
                         else
                         {

                           f_setfld ( form_sub, message, message2);

                         }
                         t_alarm( process_id );
                         return_string = t_form (form_sub, S_NULL, NIL);

                         f_rstfld ( form_sub, NO_FLAGS, message);
                         var_clr( message );

                         f_rstfld ( form_sub, NO_FLAGS, choice);
                         var_clr(choice);
                        }
                      }
                      break;


          case  OBJ_OCD_TEST  :

                      s_init ( &form_sub, "obj_ocd"    );
                      return_string = t_form (form_sub, S_NULL, NIL );


                      while ( return_string != S_NULL         &&
                              ! ( t_ifquit ( return_string ))   )
                      {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                         f_rstfld ( form_sub, NO_FLAGS, choice );
                         var_clr(choice);


                         while (  sub_lvl_rtn > OBJ_OCD_RTNS      ||
                                  sub_lvl_rtn < 1                  )
                         {

                            if ( sub_lvl_rtn != 0 )
                            {

                               f_setfld (form_sub, message, message1);
                               t_alarm ( process_id );

                            }


                            return_string = t_form( form_sub,S_NULL,NIL);

                            if ( return_string == S_NULL         ||
                                 t_ifquit ( return_string )        )
                            {

                                break;

                             }
                             else
                             {

                               s_stosi(&sub_lvl_rtn,var_get(choice),1,2);
                               #D_show ('sublevel =%d', 'sub_lvl_rtn')  ;

                               f_rstfld ( form_sub, NO_FLAGS, message)  ;
                               var_clr( message );

                               f_rstfld ( form_sub, NO_FLAGS, choice);
                               var_clr(choice);

                             }
                            }

                           if ( return_string != S_NULL          &&
                                ! ( t_ifquit ( return_string ))    )
                           {

                           switch ( sub_lvl_rtn )
                           {


                           case   1 :
                                    rc = tocdbse ();
                                    rc = tocdctos();
                                    rc = tocddbp ();
                                    rc = tocddcd ();
                                    rc = tocddrp ();
                                    rc = tocdini ();
                                    rc = tocdkey ();
                                    rc = tocdlen ();
                                    rc = tocdnew ();
                                    rc = tocdnext();
                                    rc = tocdprev();
                                    rc = tocdroot();
                                    rc = tocdsgln();
                                    rc = tocdstoc();
                                    break;
                           case   2 :
                                    rc = tocdbse ();
                                    break;
                           case   3 :
                                    rc = tocdctos();
                                    break;
                           case   4 :
                                    rc = tocddbp ();
                                    break;
                           case   5 :
                                    rc = tocddcd ();
                                    break;
                           case   6 :
                                    rc = tocddrp ();
                                    break;
                           case   7 :
                                    rc = tocdini ();
                                    break;
                           case   8 :
                                    rc = tocdkey ();
                                    break;
                           case   9 :
                                    rc = tocdlen ();
                                    break;
                           case  10 :
                                    rc = tocdnew ();
                                    break;
                           case  11 :
                                    rc = tocdnext();
                                    break;
                           case  12 :
                                    rc = tocdprev();
                                    break;
                           case  13 :
                                    rc = tocdroot();
                                    break;
                           case  14 :
                                    rc = tocdsgln();
                                    break;
                           case  15 :
                                    rc = tocdstoc();
                                    break;

                           }
                           if ( rc != 0 )
                           {

                              f_setfld ( form_sub, message, message3);

                           }
                           else
                           {

                              f_setfld ( form_sub, message, message2);

                           }
                           t_alarm( process_id );
                           return_string = t_form( form_sub, S_NULL,NIL);

                           f_rstfld ( form_sub, NO_FLAGS, message);
                           var_clr( message );

                           f_rstfld ( form_sub, NO_FLAGS, choice);
                           var_clr(choice);
                         }
                       }
                       break;


          case  OBJ_SOIDI_TEST  :

                      s_init ( &form_sub, "obj_soidi"    );
                      return_string = t_form (form_sub, S_NULL, NIL);


                      while ( return_string != S_NULL           &&
                              ! ( t_ifquit ( return_string ))     )
                      {

                        rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                        f_rstfld ( form_sub, NO_FLAGS, choice );
                        var_clr(choice);


                        while (  sub_lvl_rtn > OBJ_SOI_RTNS   ||
                                 sub_lvl_rtn < 1 )
                        {


                           if ( sub_lvl_rtn != 0 )
                           {

                               f_setfld (form_sub, message, message1);
                               t_alarm ( process_id );

                           }


                           return_string = t_form( form_sub, S_NULL,NIL);

                           if ( return_string == S_NULL           ||
                                t_ifquit ( return_string )          )
                           {

                              break;

                           }
                           else
                           {

                             s_stosi(&sub_lvl_rtn,var_get(choice),1,2);
                             #D_show ('sublevel =%d', 'sub_lvl_rtn');

                             f_rstfld ( form_sub, NO_FLAGS, message);
                             var_clr( message );

                             f_rstfld ( form_sub, NO_FLAGS, choice);
                             var_clr(choice);

                           }
                         }

                        if ( return_string != S_NULL         &&
                             ! ( t_ifquit ( return_string ))   )
                        {

                        switch ( sub_lvl_rtn )
                        {


                        case   1 :
                                 rc = tsoidiad();
                                 rc = tsoiddl ();
                                 rc = tsoiddu () ;
                                 rc = tsoidigt ();
                                 break;
                        case   2 :
                                 rc = tsoidiad();
                                 break;
                        case   3 :
                                 rc = tsoiddl ();
                                 break;
                        case   4 :
                                 rc = tsoiddu () ;
                                 break;
                        case   5 :
                                 rc = tsoidigt ();
                                 break;

                           }
                           if ( rc != 0 )
                           {

                              f_setfld ( form_sub, message, message3);

                           }
                           else
                           {

                             f_setfld ( form_sub, message, message2);

                           }
                           t_alarm( process_id );
                           return_string = t_form( form_sub, S_NULL,NIL);

                           f_rstfld ( form_sub, NO_FLAGS, message);
                           var_clr( message );

                           f_rstfld ( form_sub, NO_FLAGS, choice);
                           var_clr(choice);
                         }
                       }
                       break;

          case OBJ_OID_TEST   :

                      s_init ( &form_sub, "obj_oid"    );
                      return_string = t_form (form_sub, S_NULL, NIL);


                      while ( return_string != S_NULL            &&
                              ! ( t_ifquit ( return_string ))      )
                      {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                         f_rstfld ( form_sub, NO_FLAGS, choice );
                         var_clr(choice);


                         while (  sub_lvl_rtn > OBJ_OID_RTNS  ||
                                  sub_lvl_rtn < 1               )
                         {

                           if ( sub_lvl_rtn != 0 )
                           {

                              f_setfld (form_sub, message, message1);
                              t_alarm ( process_id );

                           }


                           return_string = t_form(form_sub,S_NULL,NIL);

                           if ( return_string == S_NULL           ||
                                t_ifquit( return_string )           )
                           {

                              break;

                           }
                           else
                           {

                             s_stosi(&sub_lvl_rtn,var_get(choice),1,2)  ;
                             #D_show ('sublevel =%d', 'sub_lvl_rtn');

                             f_rstfld ( form_sub, NO_FLAGS, message);
                             var_clr( message );

                             f_rstfld ( form_sub, NO_FLAGS, choice);
                             var_clr(choice);

                            }
                          }

                         if ( return_string != S_NULL               &&
                              ! ( t_ifquit ( return_string ))         )
                         {
                         switch ( sub_lvl_rtn )
                         {


                          case   1 :
                                   rc = toidnew ();
                                   rc = toidkey ();
                                   rc = toidcmp ();
                                   break;
                          case   2 :
                                   rc = toidnew ();
                                   break;
                          case   3 :
                                   rc = toidkey ();
                                   break;
                          case   4 :
                                   rc = toidcmp ();
                                   break;


                            }
                            if ( rc != 0 )
                            {


                               f_setfld ( form_sub, message, message3);

                             }
                             else
                             {

                               f_setfld ( form_sub, message, message2);

                             }

                             t_alarm( process_id );
                             return_string =t_form(form_sub,S_NULL,NIL);

                             f_rstfld ( form_sub, NO_FLAGS, message);
                             var_clr( message );

                             f_rstfld ( form_sub, NO_FLAGS, choice);
                             var_clr(choice);
                           }
                       }
                       break;

          case OBJ_PSI_TEST  :

                      s_init ( &form_sub, "obj_psi"    );
                      return_string = t_form (form_sub, S_NULL, NIL);

                      while ( return_string != S_NULL         &&
                              ! ( t_ifquit ( return_string ) )  )
                      {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                         f_rstfld ( form_sub, NO_FLAGS, choice );
                         var_clr(choice);


                         while (  sub_lvl_rtn > OBJ_PSI_RTNS         ||
                                  sub_lvl_rtn < 1                     )
                         {

                            if ( sub_lvl_rtn != 0 )
                            {

                                f_setfld (form_sub, message, message1);
                                t_alarm ( process_id );

                            }


                            return_string = t_form(form_sub,S_NULL,NIL);

                            if ( return_string == S_NULL          ||
                                 t_ifquit( return_string )          )
                            {

                               break;

                            }
                            else
                            {

                              s_stosi(&sub_lvl_rtn,var_get(choice),1,2);
                              #D_show ('sublevel =%d', 'sub_lvl_rtn');

                              f_rstfld ( form_sub, NO_FLAGS, message);
                              var_clr( message );

                              f_rstfld ( form_sub, NO_FLAGS, choice);
                              var_clr(choice);

                            }
                          }

                         if ( return_string != S_NULL           &&
                              ! t_ifquit ( return_string )          )
                         {

                         switch ( sub_lvl_rtn )
                         {


                         case   1 :
                                  rc = tpsiadd ();
                                  rc = tpsidump();
                                  break;
                         case   2 :
                                  rc = tpsiadd ();
                                  break;
                         case   3 :
                                  rc = tpsidump();
                                  break;

                         }
                         if ( rc != 0 )
                         {

                           f_setfld ( form_sub, message, message3);

                         }
                         else
                         {

                            f_setfld ( form_sub, message, message2);

                         }
                         t_alarm( process_id );
                         return_string = t_form( form_sub,S_NULL,NIL);

                         f_rstfld ( form_sub, NO_FLAGS, message);
                         var_clr( message );

                         f_rstfld ( form_sub, NO_FLAGS, choice);
                         var_clr(choice);
                       }
                     }
                     break;


          case  OBJ_LST_TEST  :

                      s_init ( &form_sub, "obj_lst"    );
                      return_string = t_form (form_sub, S_NULL, NIL );


                      while ( return_string != S_NULL         &&
                              ! ( t_ifquit ( return_string ))   )
                      {

                         rc =s_stosi(&sub_lvl_rtn,var_get(choice),1,2);

                         f_rstfld ( form_sub, NO_FLAGS, choice );
                         var_clr(choice);


                         while (  sub_lvl_rtn > OBJ_LST_RTNS      ||
                                  sub_lvl_rtn < 1                  )
                         {

                            if ( sub_lvl_rtn != 0 )
                            {

                               f_setfld (form_sub, message, message1);
                               t_alarm ( process_id );

                            }


                            return_string = t_form( form_sub,S_NULL,NIL);

                            if ( return_string == S_NULL         ||
                                 t_ifquit ( return_string )        )
                            {

                                break;

                             }
                             else
                             {

                               s_stosi(&sub_lvl_rtn,var_get(choice),1,2);
                               #D_show ('sublevel =%d', 'sub_lvl_rtn')  ;

                               f_rstfld ( form_sub, NO_FLAGS, message)  ;
                               var_clr( message );

                               f_rstfld ( form_sub, NO_FLAGS, choice);
                               var_clr(choice);

                             }
                            }

                           if ( return_string != S_NULL          &&
                                ! ( t_ifquit ( return_string ))    )
                           {

                           switch ( sub_lvl_rtn )
                           {


                           case   1 :
                                    rc = tablist ();
                                    rc = tcrlist ();
                                    rc = tdolist ();
                                    rc = texlist ();
                                    rc = tfnlist ();
                                    rc = tgtlist ();
                                    rc = tjnlist ();
                                    rc = tlistfun();
                                    rc = tlklist ();
                                    rc = tmdlist ();
                                    rc = torlist ();
                                    rc = trclist ();
                                    rc = trmlist ();
                                    rc = trvlist ();
                                    rc = tsclist ();
                                    break;
                           case   2 :
                                    rc = tablist ();
                                    break;
                           case   3 :
                                    rc = tcrlist ();
                                    break;
                           case   4 :
                                    rc = tdolist ();
                                    break;
                           case   5 :
                                    rc = texlist ();
                                    break;
                           case   6 :
                                    rc = tfnlist ();
                                    break;
                           case   7 :
                                    rc = tgtlist ();
                                    break;
                           case   8 :
                                    rc = tjnlist ();
                                    break;
                           case   9 :
                                    rc = tlistfun();
                                    break;
                           case  10 :
                                    rc = tlklist ();
                                    break;
                           case  11 :
                                    rc = tmdlist ();
                                    break;
                           case  12 :
                                    rc = torlist ();
                                    break;
                           case  13 :
                                    rc = trclist ();
                                    break;
                           case  14 :
                                    rc = trmlist ();
                                    break;
                           case  15 :
                                    rc = trvlist ();
                                    break;
                           case  16 :
                                    rc = tsclist ();
                                    break;

                           }
                           if ( rc != 0 )
                           {

                              f_setfld ( form_sub, message, message3);

                           }
                           else
                           {

                              f_setfld ( form_sub, message, message2);

                           }
                           t_alarm( process_id );
                           return_string = t_form( form_sub, S_NULL,NIL);

                           f_rstfld ( form_sub, NO_FLAGS, message);
                           var_clr( message );

                           f_rstfld ( form_sub, NO_FLAGS, choice);
                           var_clr(choice);
                         }
                       }
                       break;


        default     :
                    break;

            }
          }
        }



#D_return( rc );
}
