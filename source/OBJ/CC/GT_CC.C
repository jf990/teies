/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    gt_cc                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 28, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   cc_ocb      OCB *      INPUT                           |
  |  <PARAM>   conf_ocb    OCB *      INPUT                           |
  |  <PARAM>   flags       hword      INPUT                           |
  |  <PARAM>   memb_ocb    OCB *      INPUT                           |
  |                                                                   |
  |  <PURPOSE> Get Conference Comment                                 |
  |            If OCB is conf obj, then put up form for specific      |
  |            comment.                                               |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "tm.h"
#include "form.h"
#include "obj_prim.h"


sint  gt_cc(  cc_ocb, conf_ocb, flags, memb_ocb  )

OCB         * cc_ocb,
            * conf_ocb,
            * memb_ocb;
hword         flags;

{

sint      rc          = NO_ERROR,
          count       =      0;
string    form_id     = S_NULL,
          count_str   = S_NULL,
          CONF_ID     = S_NULL,
          conf_id     = S_NULL;
OCD     * reply_ocd   =    NIL;
hword     key_press   =      0;
boolean   done        =  FALSE;






if( ( ! OCB_POINTER_OK( cc_ocb ) )  &&  ( cc_ocb->type != COMMENT_OBJ ) )
{

   rc = vw_cr( NIL, S_NULL, flags, NIL );

}
else
{

   s_init ( & form_id,  "1151"         );
   s_init ( & CONF_ID,  "CONF_ID"      );
   obj_find(  conf_ocb, "CONFERENCE.ID", & conf_id );
   var_set(   CONF_ID,   conf_id       );

   while(  ( rc == NO_ERROR ) && NOT done  )
   {

      key_press = t_form( form_id, S_NULL, cc_ocb );

      switch( key_press )
      {

         case FORM_NOT_FORMED        :
         case USER_REQUESTED_ABANDON :
         case USER_REQUESTED_QUIT    :

            rc   = key_press;
            done =      TRUE;
            break           ;

         case USER_REQUESTED_CONFIRM :

            rc   = NO_ERROR;
            done =    FALSE;
            break          ;

         case USER_REQUESTED_ENTER   :
         case USER_REQUESTED_PROCESS :

            rc   = rc_cc( cc_ocb, conf_ocb, flags, memb_ocb );
            done = TRUE;
            break;

         case USER_REQUESTED_REPLY:

            done = FALSE;
            rc = cr_cc( conf_ocb, (string) memb_ocb, flags | DOING_REPLY,
                                                              & cc_ocb );
            switch(  rc  )
            {

               case USER_REQUESTED_QUIT :

                  rc = NO_ERROR;
                  break;

               case NO_ERROR               :
               case USER_REQUESTED_PROCESS :
               case USER_REQUESTED_ENTER   :

                  ocd_new( & reply_ocd, "COMMENT.REPLY_COUNT" );
                  obj_get(   cc_ocb,    reply_ocd, & count    );
                  s_sitos( & count_str, count                 );
                  var_set(   reply_ocd->name, count_str       );
                  f_setfld(  form_id,   reply_ocd->name, count_str );

                  s_drop(  & count_str );
                  ocd_drp( & reply_ocd );
                  break;

               default:

                  break;

            }
            break;

         case USER_REQUESTED_KEEP:

            done =     TRUE;
            rc   = NO_ERROR;
            break;

         case USER_REQUESTED_SCAN:

            done = FALSE;
            rc   = do_act( cc_ocb, S_NULL, flags, NULL );
            if( rc == USER_REQUESTED_QUIT )
            {
               rc = NO_ERROR;
            }
            break;

         default:

            done =     FALSE;
            rc   = key_press;
            break;
      }
   }


   f_close(   form_id, STATE_FORM );
   s_drop ( & form_id             );
   var_clr(   CONF_ID             );
   s_drop ( & CONF_ID             );
   s_drop ( & conf_id             );

}  /* if cc ocb was good */


   return(rc);

}
