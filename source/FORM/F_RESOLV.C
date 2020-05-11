/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_resolv.c                                             |
  |                                                                   |
  |  :AUTHOR.  Nestor Voronka                                         |
  |                                                                   |
  |  :DATE.    January 6, 1987                                        |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   form_id   string      INPUT   Form identification.     |
  |                                                                   |
  |  :PURPOSE. Determine if the form has already been parsed, if so   |
  |            returns the pointer to the structure that contains the |
  |            Presentation Manager's profile.  Else returns NULL.    |
  |                                                                   |
  |  :OUTPUT.  Returns a pointer or NULL.                             |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.                                                         |
  |                                                                   |
  |  :LIMITS.                                                         |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"

form_dat *f_resolv (form_id, flags)
string  form_id ;
hword   flags ;
{
   form_dat *formdata = NIL ;    /* Pointer to Form data structure    */

                                  /* Check to see if form is in table  */
   formdata = frm_inta (form_id,flags) ;
                                  /* See if it was found or not        */
   if (formdata == NIL || formdata == (form_dat *) -1 )
   {

      formdata = NIL ;

   }
   else
   {                              /* Copy the pointer to p_prof        */

      ((struct form_process *) TM_master->current->forms)->p_prof =
                                                       formdata->p_prof ;

   }


   return(formdata);
}
