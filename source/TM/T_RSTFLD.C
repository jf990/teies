/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    t_rstfld.c                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   TM                                                     |*/
/*|                                                                   |*/
/*|  :AUTHOR.  John L. Foster                                         |*/
/*|                                                                   |*/
/*|  :DATE.    August  15, 1988                                       |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   string form_id        INPUT  Form id string so we know |*/
/*|                                         which form to get the data|*/
/*|                                         fields from.              |*/
/*|                                                                   |*/
/*|  :PURPOSE. Once the processing for a given form is complete we    |*/
/*|            reset all the fields to the contents they had before   |*/
/*|            the user started mucking around with it.  This way,    |*/
/*|            in case the form is requested again and it has not been|*/
/*|            closed we guarentee a fresh start.                     |*/
/*|                                                                   |*/
/*|  :OUTPUT.  sint          always 0                                 |*/
/*|  :CALLS.                                                          |*/
/*|  :ERRORS.                                                         |*/
/*|  :LIMITS.                                                         |*/
/*|  :MODS.                                                           |*/
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





string          t_rstfld( form_id )

string                    form_id;


{


   sint         rc            =   0 ;
   INF         *current_field =  NIL;
   form_dat    *fcb           =  NIL;





   if( FORM_NOT_ACTIVE != ( fcb = frm_inta( form_id, NO_FLAGS )) )
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if( ( current_field->ftype == FLD_INPUT )  )
         {

            s_copy( &(current_field->contents),
                                         current_field->init_val, 0, 0 );

         }
         current_field = current_field->next;

      }
   }


   return(0);
}
