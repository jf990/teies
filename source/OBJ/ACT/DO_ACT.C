/************************************************************************
    <DOC>
    <NAME>         do_act( OCB *attach_ocb, string modifier, hword level,
                                                           OCB **result )
    <LEVEL>        Activity
    <DATE>         April 5, 1988     March 1, 1989
    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar
    <PARAM>        attach_ocb OCB *   INPUT  The ocb of the object to
                                             which the activity is
                                             attached.
                   modifier   string  INPUT
                   level      hword   INPUT
                   result     OCB **  IN/OUT
    <OUTPUT>       Sint.
    <PURPOSE>      This routine will allow a user to do an activity.
    <CALLS>        a_crrd(), a_crqs(), a_crsl(), obj_opn(), Obj_upd(),
                   stosi(), s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), os_memn(), t_form(), isv_match(),
                   var_clr().
    <ERRORS>
                   ACTIVITY_ERROR             6400
                   NO_ASSOCIATED_ACTIVITY     6403
                   ACTIVITY_IS_CLOSED         6404
                   INVALID_OCB               20022
                   FUNCTION_NOT_SUPPORTED    20222
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
#include "activity.h"

sint do_act( attach_ocb, modifier, level, result )

OCB     *attach_ocb;
string  modifier;
hword   level;
OCB     **result;

{
   OCB        *act_ocb        =         NIL;

   OCD        *ocd            =         NIL,
              *act_ocd        =         NIL;

   OID        *act_oid        =         NIL;

   hword      type            =           0;

   sint       rc              =           0,
              xc              =           0;

   string     state           =      S_NULL,
              closed          =      S_NULL;

   byte       status          =        '\0';



   if (OCB_POINTER_OK( attach_ocb ) )
   {
      if ( (attach_ocb->type) == MAIL_OBJ )
      {
         ocd_new( &ocd, "MESSAGE.ACTIVITY" );
      }
      else if ( (attach_ocb->type) == COMMENT_OBJ )
      {
         ocd_new( &ocd, "COMMENT.ACTIVITY" );
      }
      else
      {
         rc = FUNCTION_NOT_SUPPORTED;
      }
   }
   else
   {
      rc = INVALID_OCB;
   }

   if( rc == 0)
   {
      act_oid = ( OID * )os_mema( sizeof (OID) );
      obj_get( attach_ocb, ocd, act_oid );
      act_ocb = obj_opn( act_oid, OBJ_UPDATE | OBJ_REMOTE);
      if (OCB_POINTER_OK (act_ocb) )
      {
         obj_find( act_ocb, "ACTIVITY.STATUS", &status );
         s_ctos( &state, status );
         isv_get( &closed, ISV_CLOSED, 0);
         if ( s_comp( state, closed, 1, 1, 'F', &xc ) == 0 )
         {
            rc = ACTIVITY_IS_CLOSED;
            except(rc, USER_INFO, S_NULL, S_NULL, S_NULL, S_NULL,S_NULL);
         }
         else
         {
            s_drop(&state);
            obj_set_flag(act_ocb, OBJ_RAW);
            obj_find( act_ocb, "ACTIVITY.ISV_TYPE", &type );
            obj_reset_flag(act_ocb, OBJ_RAW);


            switch ( type )            /* type of activity to create */

            {
               case ISV_READ:             /* READ activity            */

                   rc = a_dord(act_ocb, (sint)attach_ocb->type,
                                                   attach_ocb, &act_ocb);
                   break;

               case ISV_QUESTION:      /* QUESTION Discussion activity */

                   rc = a_doqs(act_ocb, (sint)attach_ocb->type,
                                                   attach_ocb, &act_ocb);
                   break;

               case ISV_SELECTIO:        /* SELECTION activity         */

                   rc = a_dosl(act_ocb, (sint)attach_ocb->type,
                                                   attach_ocb, &act_ocb);
                   break;

               default:
                   rc = ACTIVITY_ERROR;
                   break;

            }                            /* end switch statement */
         }
         if (OCB_POINTER_OK (act_ocb) )
         {
            obj_cls( act_ocb );
         }
      }
      else
      {
         rc = NO_ASSOCIATED_ACTIVITY;
         except(rc, USER_INFO, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);
      }
   }                                         /* end of if-statement */
   ocd_drp(&ocd);
   os_memd(act_oid);

   return(rc);
}
