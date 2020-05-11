/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_edtscr()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 19, 1989                                          |
  |  <LEVEL>   TM                                                     |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   state_id    string     INPUT  State to determine form. |
  |  <PARAM>   flags       hword      INPUT  Type of form.            |
  |  <PARAM>   options     sint       INPUT  No yet implemented.      |
  |                                                                   |
  |  <PURPOSE> Call on the editor to edit a TEIES form (either HELP   |
  |            or FORM types).                                        |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   t_spad()                                               |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "tm.h"
#include "form.h"




sint   t_edtscr(  state_id, flags, options  )

string            state_id;
hword             flags;
sint              options;

{

   sint        rc           =      0,
               end_function =      0;

   string      gfd_prefix   = S_NULL,
               form_id      = S_NULL;

   byte      * form_type    =    NIL;




   s_init ( & gfd_prefix, "* * "                  ) ;
   s_copy ( & form_id,    gfd_prefix,        0, 0 ) ;
   s_cat  ( & form_id,    form_id, state_id, 0, 0 ) ;

   if (  CHK_MASK( flags, HELP_FORM )  )
   {

      form_type = " HELP" ;

   }
   else
   {

      form_type = " FORM";

   }
   s_init ( & gfd_prefix, form_type );
   s_cat  ( & form_id,    form_id, gfd_prefix , 0, 0 );


   end_function = t_spad( form_id, S_NULL );

   s_drop ( & gfd_prefix   ) ;
   s_drop ( & form_id      ) ;


   return(rc);

}
