/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    t_updfld.c                                             |*/
/*|  <LEVEL>   TM                                                     |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <DATE>    January 15, 1988                                       |*/
/*|  <PARAM>   string form_id        INPUT  Form id string so we know |*/
/*|                                         which form to get the data|*/
/*|                                         fields from.              |*/
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
/*|  <MODS>    10_Aug_88  JF   Improved logic to take into account    |*/
/*|                            the field update bit from the INF and  |*/
/*|                            added the reset of the update bit.     |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "debug.h"
#include "form.h"





sint            t_updfld( form_id )

string                    form_id;


{


   sint         rc            =   0 ,
                field_counter =   0 ;
   form_dat    *fcb           =  NIL;
   INF         *current_field =  NIL;
   string       last_field    =S_NULL,
                holder        =S_NULL,
                command_line  =S_NULL;





   fcb = frm_inta( form_id, NO_FLAGS );
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
            s_copy( &(current_field->prv_cont), holder, 0, 0 );

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

   s_init( & command_line, "%CMD" );
   if( f_rstfld( form_id, NO_FLAGS, command_line ))
   {                                               /********************
                                                    * Only attempt to  *
                                                    * check the command*
                                                    * line if one was  *
                                                    * on the form.  The*
                                                    * field is cleared *
                                                    * so we dont execut*
                                                    * it again next tme*
                                                    ********************/

      s_copy(  & holder, (string) var_get( command_line ), 0, 0 );
      s_ctrim( & holder, 'B', ' ' );
      s_ctrim( & holder, 'B', '_' );
      var_clr(   command_line     );
      rc = t_command( holder, (struct memnode *)TM_master->uicmdtab, 0 );

   }
   s_drop(  & command_line );
   s_drop(  & holder       );

   return(rc);
}
