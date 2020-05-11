/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    f_updfld                                               |*/
/*|  <LEVEL>   FORMS                                                  |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <DATE>    March 14, 1989                                         |*/
/*|                                                                   |*/
/*|  <PARAM>   string form_id        INPUT  Form id string so we know |*/
/*|                                         which form to get the data|*/
/*|                                         fields from.              |*/
/*|                                                                   |*/
/*|  <PARAM>   hword  form_flags     INPUT  Standard Form level flag  |*/
/*|                                         bit switches.             |*/
/*|                                                                   |*/
/*|  <PURPOSE> After a form is displayed and a user interacts with it,|*/
/*|            we need to step through all the fields to assure they  |*/
/*|            have been entered according to rules and restrictions  |*/
/*|            set by the <field> markup and field data type.         |*/
/*|                                                                   |*/
/*|            Data type general rules are:                           |*/
/*|                                                                   |*/
/*|            String (default) : contains only printable characters. |*/
/*|                Min and Max values specified apply to the minimum  |*/
/*|                and maximum length of the string.  Default min is 0|*/
/*|                max is 78.                                         |*/
/*|                                                                   |*/
/*|            Number : contains only decimal digits. Decimal point   |*/
/*|                and sign are optional. Min and Max denote value    |*/
/*|                range.                                             |*/
/*|                                                                   |*/
/*|            Date : must be a valid date according to the users date|*/
/*|                and time format string.  Minimum and maximum are   |*/
/*|                used to indicate min and max date values.          |*/
/*|                                                                   |*/
/*|            Command Line:  If the form has a command line on it    |*/
/*|                then we process it here.  Once processed it is also|*/
/*|                cleared.                                           |*/
/*|                                                                   |*/
/*|  <OUTPUT>  string  NIL indicates all fields updated properly;     |*/
/*|                     -1 indicates TEIES signoff requested;         |*/
/*|                    ptr indicates the field number with the first  |*/
/*|                          problem.                                 |*/
/*|  <CALLS>                                                          |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"





sint            f_updfld( form_id, form_flags )

string                    form_id;
hword                     form_flags;


{


   sint         rc            =    0 ,
                field_counter =    0 ;
   form_dat    *fcb           =   NIL;
   INF         *current_field =   NIL;
   string       last_field    =S_NULL,
                holder        =S_NULL;





   fcb = frm_inta( form_id, form_flags );
   if( fcb != FORM_NOT_ACTIVE )
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if( ( current_field->ftype == FLD_INPUT )  )
         {

            s_copy(  & holder, current_field->contents, 0, 0   );
            s_ctrim( & holder, 'B', current_field->pad         );
            s_ctrim( & holder, 'B', ' '                        );

            if( current_field->ocb != NIL )
            {

               ocd_stoc(current_field->ocb, current_field->ocd, holder );

            }
            else
            {

               var_set( current_field->ocd->name, holder     );

            }
            s_copy( &(current_field->prv_cont),
                      current_field->contents,
                      0, 0
                  );

            if( s_len( holder ) < 1 )    /* + + + + + + + + + + + + + */
            {                            /* If the resulting field    */
                                         /* has nothing in it, reset  */
                                         /* it to contain what it had */
                                         /* when the form was first   */
                                         /* parsed, BUT only after we */
                                         /* update the variable to the*/
                                         /* empty contents!           */
                                         /* + + + + + + + + + + + + + */

               s_copy( &(current_field->contents),
                         current_field->init_val, 0, 0 );

            }
            ++field_counter;

         }
         current_field = current_field->next;

      }
   }

   s_drop(  & holder       );

   return(rc);
}
