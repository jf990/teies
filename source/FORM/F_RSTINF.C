/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_rstinf()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS                                                  |*/
/*|                                                                   |*/
/*|  :AUTHOR.  John L. Foster                                         |*/
/*|                                                                   |*/
/*|  :DATE.    August 11, 1988                                        |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   string form_id        INPUT  Form id string so we know |*/
/*|                                         which form to get the data|*/
/*|                                         fields from.              |*/
/*|                                                                   |*/
/*|  :PARAM.   hword  flags          INPUT  Form flags                |*/
/*|                                                                   |*/
/*|  :PURPOSE. Before a form is displayed all the fields should be set|*/
/*|            to not-modified.  This will be the case the first time |*/
/*|            the form goes up so if the form does not exist we just |*/
/*|            return and do not report an error.                     |*/
/*|                                                                   |*/
/*|  :OUTPUT.  sint                   always  0                       |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.                                                           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "debug.h"
#include "form.h"







sint            f_rstinf( form_id, flags )

string                    form_id;
hword                     flags;


{



   sint         rc            =   0 ,
                field_counter =   0 ;
   form_dat    *fcb           =  NIL;
   INF         *current_field =  NIL;
   string       last_field    =S_NULL,
                holder        =S_NULL,
                command_line  =S_NULL;







   fcb = frm_inta( form_id, flags );
   if( fcb != FORM_NOT_ACTIVE && fcb != NIL )
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if( current_field->ftype == FLD_INPUT )
         {

            current_field->flags &= ~ F_MOD;

         }
         current_field = current_field->next;

      }
   }

   return(rc);
}
