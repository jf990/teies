/************************************************************************
    <DOC>
    <NAME>         cr_act(OCB *atch_ocb, string  modifier,hword level,
                                                          OCB **result)
    <LEVEL>        Activity
    <DATE>         Feb 12, 1988      March 1, 1989
    <AUTHOR>       Heidi E. Harting  Tanmay S. Kumar
    <PARAM>        atch_ocb   OCB *   INPUT  The ocb of the object to
                                             which the activity will be
                                             attached.
                   modifier   string  INPUT  Not used
                   level      hword   INPUT  Not used
                   result     OCB **  IN/OUT Used to pass back the Act
                                             object control block(OCB)
    <OUTPUT>       Sint.
    <PURPOSE>      This routine will first determine what the activity
                   will be attached to, a message or conference comment.
                   From there, it will call the routine to service the
                   creation of an activity for that object.
                   This routine will start by displaying the first screen
                   of the 'Create Activity State'. The user will select
                   the type of activity he/she would like to attach to a
                   conference comment or message, and this routine will
                   call the appropriate subordinate routine accordingly.
    <CALLS>        a_crrd(), a_crqs(), a_crsl(), obj_opn(), obj_upd(),
                   stosi(), s_init(), s_drop(), s_comp(), f_curpos(),
                   f_close(), except(), os_memn(), t_form(), isv_match(),
                   var_clr(), except().
    <ERRORS>
                   INVALID_CHOICE            7905
                   FINCTION_NOT_SUPPORTED    20222
    <FORMS>        A_CRM and A_CRC
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

sint  cr_act( atch_ocb, modifier, level, result )

OCB     *atch_ocb;
string  modifier;
hword   level;
OCB     **result;

{
   OCB      *ocb          =    NIL,
            *act_ocb      =    NIL;

   string   form          = S_NULL,
            type          = S_NULL;

   sint     rc            =      0,
            a_type        =      0,       /* user input */
            part_count    =      0,
            xc            =      0;

   hword    act_type      =      0;

   OID      act_oid;

   boolean  flag          =  FALSE;


   #D_begin(cr_act, ACTIVITY)

   s_init( &type, "TYPE" );
   os_memn( &act_oid, sizeof(OID));

   if ( atch_ocb->type == MAIL_OBJ )
   {
      s_init( &form, "A_CRM" );
   }
   else if ( atch_ocb->type == COMMENT_OBJ )
   {
      s_init( &form, "A_CRC" );
   }
   else
   {
      rc = FUNCTION_NOT_SUPPORTED;
      except( rc, USER_ERROR, S_NULL, S_NULL, S_NULL, S_NULL, S_NULL);
   }

   if ( rc == 0)
   {

      f_curpos( form, NO_FLAGS, type, 0.0, 0.0 );
      while (!flag)
      {
         rc = t_form( form, S_NULL, NIL );

         if ( ( rc == USER_REQUESTED_QUIT )
         || (rc == USER_REQUESTED_ABANDON) || (rc == TM_FORM_NOT_FORMED))
         {
            flag = TRUE;
         }

         else if ( ( rc == USER_REQUESTED_PROCESS )
               ||  ( rc == USER_REQUESTED_ENTER ) )
         {
            rc = 0;
            s_stosi( &a_type, var_get(type), 0, 0 ); /* activity type */
            #D_show('The Activity Type: %d', a_type)

            switch ( a_type )       /* type of activity to create */
            {

             case READ:                     /* READ activity */
                 act_type = ISV_READ;
                 rc = a_crrd( atch_ocb, &act_oid );
                 break;

             case QUESTION:            /* QUESTION Discussion activity */
                 act_type = ISV_QUESTION;
                 rc = a_crqs( atch_ocb, &act_oid );
                 break;

             case SELECTION:                /* SELECTION activity */
                 act_type =  ISV_SELECTIO;
                 rc = a_crsl( atch_ocb, &act_oid );
                 break;

             default:
                 rc = INVALID_CHOICE;
                 except( rc, USER_ERROR, var_get(type), S_NULL, S_NULL,
                                                        S_NULL, S_NULL);
                 f_curpos( form, NO_FLAGS, type, 0.0, 0.0 );
                 continue;
                 break;
            }                                 /* end switch statement */

            #D_show('the return code rc = %d',rc);

            if ( ( rc == 0 ) || (rc == USER_REQUESTED_ABANDON) )
            {
               #D_oid( '(&(act_oid))', 'Activity OID' );
               flag = TRUE;
            }
            else
            {
               rc = 0;
               continue;
            }
         }
         else
         {
            continue;
         }
      }                      /* exit from this form  (while loop) */

      #D_show('isv of act_type = %d', act_type);
      f_close( form, NO_FLAGS);

      if ( (rc == 0) && ( OID_POINTER_OK (&act_oid) ) )
      {
         act_ocb = obj_opn(&act_oid, OBJ_UPDATE );
         #D_oid( '(&(atch_ocb->oid))', 'OID of attachment' );
         #D_ocb(act_ocb);
         obj_find(act_ocb, "ACTIVITY.PART_COUNT", &part_count);
         if ( atch_ocb->type == MAIL_OBJ )
         {
            obj_upd( atch_ocb, "MESSAGE.ACTIVITY", &act_oid );
            obj_upd( atch_ocb, "MESSAGE.ACT_TYPE", &act_type );
            obj_upd( atch_ocb, "MESSAGE.A_SEC_COUNT", &part_count);
         }
         else if ( atch_ocb->type == COMMENT_OBJ )
         {
            obj_upd( atch_ocb, "COMMENT.ACTIVITY", &act_oid );
            obj_upd( atch_ocb, "COMMENT.ACT_TYPE", &act_type );
            obj_upd( atch_ocb, "COMMENT.A_SEC_COUNT", &part_count);
         }
         else
         {
            ;
         }

         /* make the owner of the activity the mail or CC object */

         obj_upd( act_ocb, "%LINKS.OWNER", &(atch_ocb->oid) );
         obj_upd( act_ocb, "ACTIVITY.ATTACH_TYPE", &(atch_ocb->type) );
         obj_upd( act_ocb, "ACTIVITY.ISV_TYPE", &act_type );

         if (result != NIL)
         {
             *result = act_ocb;
         }
      }
   }

   var_clr(type);
   s_drop( &type );

   s_drop( &form );

   #D_return( rc );
}
