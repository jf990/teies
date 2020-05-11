/************************************************************************

    <DOC>

    <NAME>         a_slnot( OCB *act_ocb, sint notify, type)

    <LEVEL>        Activity

    <DATE>         March 24, 1988    March 1, 1989

    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar

    <GLOBAL>       None.

    <PARAM>        act_ocb    OCB *   INPUT  The ocb of the activity
                   notify     sint    INPUT  Identifies whether notifs
                                             are wanted or not.
                   type       sint    INPUT  Type of activity

    <OUTPUT>       Sint.

    <PURPOSE>      This routine is meant to select notification types
                   for activities.

    <CALLS>
                   s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), t_valyn(), t_form(), isv_match(),
                   isv_get(), var_clr(), var_get(), var_set(), ocd_new(),
                   ocd_drp(), obj_rep().

    <ERRORS>
                   INPUT_EXCEPTION           7904
                   YES_OR_NO_EXPECTED        7904

    <LIMITS>       None.

    <MODS>         March 1, Rewritten routine to specifications.

    <END>

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

sint a_slnot( act_ocb, notify, type )

OCB   *act_ocb;
sint  notify;
sint  type;

{
   OCD       *act_ocd      =      NIL;

   sint      rc            =        0,
             xc            =        0;

   string    form          =   S_NULL,
             yes           =   S_NULL,
             no            =   S_NULL,
             notif_first   =   S_NULL,
             notif_each    =   S_NULL,
             notif_last    =   S_NULL,
             notif_specl   =   S_NULL;

   boolean   flag          =    FALSE;

   byte      x_byte        =      '\0';


   s_init( &notif_first, "NOTIF_FIRST" );
   s_init( &notif_each,  "NOTIF_EACH" );
   s_init( &notif_last,  "NOTIF_LAST" );
   s_init( &notif_specl, "NOTIF_SPECL" );

   isv_get( &yes, ISV_YES, 1 );
   isv_get( &no, ISV_NO, 1 );

   if (notify == 1 )                  /* a Y for wanting notifications */
   {

      while (!flag)                   /* need to repeat if input error */
      {

         switch ( type )             /* the type of activity */
         {
            case READ:
                      s_init( &form, "NOTIF_RD" );
                      break;

            case QUESTION:
                      s_init( &form, "NOTIF_QS" );
                      break;

            case SELECTION:
                      s_init( &form, "NOTIF_SL" );
                      break;

            default:
                      break;
         }                        /* end of switch-statement */

         f_curpos( form, NO_FLAGS, notif_first, 0.0, 0.0 );
         rc = t_form( form, S_NULL, NIL );

         if ( ( rc == USER_REQUESTED_QUIT )
         || (rc == USER_REQUESTED_ABANDON) || (rc == TM_FORM_NOT_FORMED))
         {
            flag = TRUE;
         }

         else if ( ( rc == FORM_PROCESS )
              ||   ( rc == FORM_ENTER ) )
         {
            rc = 0;

            if ( (t_valyn( var_get(notif_first) )  == -1 ) ||
                 (t_valyn( var_get(notif_each ) )  == -1 ) ||
                 (t_valyn( var_get(notif_last ) )  == -1 ) ||
                 (t_valyn( var_get(notif_specl) )  == -1 )   )
            {
               except( INPUT_EXCEPTION, USER_ERROR, S_NULL,
                                    S_NULL, S_NULL, S_NULL, S_NULL    );
               continue;
            }


            else                          /* no errors on input */
            {
               flag = TRUE;
            }
         }
         else
         {
             continue;
         }
      }                              /* end of while-loop */
      f_close( form, NO_FLAGS);
   }                                   /* end of if-statement */

   else                      /* notifications not wanted */
   {
      var_set( notif_first, no );
      var_set( notif_each, no );
      var_set( notif_last, no );
      var_set( notif_specl, no );
   }
   if (rc == 0)
   {

      ocd_new( &act_ocd, "ACTIVITY.NOTIFS" );

      x_byte = (byte) t_valyn( var_get(notif_first) );

      x_byte |= (byte) t_valyn( var_get(notif_each) );

      x_byte |= (byte) t_valyn( var_get(notif_last) );

      x_byte = (byte) t_valyn( var_get(notif_specl) );

      obj_rep( act_ocb, act_ocd, &x_byte, SEG_UPDATE );

      ocd_drp(&act_ocd);
   }

   var_clr(notif_first);
   var_clr(notif_each );
   var_clr(notif_last );
   var_clr(notif_specl);

   s_drop(&notif_first);
   s_drop(&notif_each );
   s_drop(&notif_last );
   s_drop(&notif_specl);

   s_drop( &form );

   return(rc);
}
