/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_flderr                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 16, 1990                                       |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   state       string     INPUT  State to determine form. |
  |  <PARAM>   field       string     INPUT  Field on the form.       |
  |  <PARAM>   error_code  sint       INPUT  What is the error?       |
  |                                                                   |
  |  <PURPOSE> It has been determined that a user input error has     |
  |            occured on a form.  This routine will automate calling |
  |            the exception handler and placing the cursor on the    |
  |            field.                                                 |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   except(), f_curpos()                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"




sint   t_flderr( state, field_name, error_code )

string           state, field_name;
sint             error_code;


{

   sint          rc             =      0;
   form_dat    * fcb            =    NIL;
   INF         * current_field  =    NIL;
   string        field_contents = S_NULL;




if( s_len( state ) > 0  &&  s_len( field_name ) > 0  &&  error_code != 0 )
{

   fcb = frm_inta( state, NO_FLAGS );
   if( fcb != FORM_NOT_ACTIVE )
   {

      current_field = fcb->fst_inf;

      while( current_field != NIL )
      {

         if( ( current_field->ftype == FLD_INPUT)      &&
           ( s_comp( current_field->ocd->name, field_name, 0, 0, 'M', 0 )
              == 0 ) )
         {

            s_copy(  & field_contents, current_field->contents, 0, 0 );

         }
         current_field = current_field->next;

      }
   }
   rc = except( error_code, USER_ERROR, field_contents, S_NULL, S_NULL,
                                                        S_NULL, S_NULL );
   f_curpos( state, STATE_FORM, field_name, 0.0, 0.0 );
   s_drop( & field_contents );

}
else
{


}

   return(rc);

}
