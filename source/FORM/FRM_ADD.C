/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    frm_add                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 11, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  *TM_master->f_table  formdat IN/OUT  Base pointer to   |
  |                                                 double linked lst |
  |                                                 of forms parsed   |
  |                                                                   |
  |  :PARAM.    form_id             string  INPUT   Form ID of form   |
  |                                                 to add to list of |
  |                                                 forms parsed      |
  |                                                                   |
  |  :PURPOSE. To allocate space for the formdata structure, insert   |
  |            it into the table of parsed forms, and copy the ID.    |
  |                                                                   |
  |  :OUTPUT.  Always returns the pointer to the created structure    |
  |                                                                   |
  |  :CALLS.   frm_inta(), os_mema(), s_copy()                        |
  |                                                                   |
  |  :ERRORS.  None.                                                  |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"

form_dat * frm_add ( form_id, flags )

string               form_id ;
hword                flags ;


{
   form_dat * curfrm = NIL ,
            * newfrm = NIL ;
   struct form_global * f_global = NIL;



   curfrm = frm_inta (form_id, flags) ;
                                  /* Check if form already parsed     */
   if (curfrm != 0 && curfrm != (form_dat *) -1 )
   {

   return(curfrm);

   }
   f_global = (struct form_global *) TM_master->forms ;

                                  /* Get PTR to last form in table     */
   curfrm = frm_inta ( (string) -1, NO_FLAGS ) ;
                                  /* Allocate space for new form_dat   */
   newfrm = (form_dat *)os_mema (sizeof (form_dat)) ;
                                  /* If table is empty                 */
   if (curfrm == NIL)
   {
      f_global->f_table = (byte *) newfrm ;
      newfrm->prev       =            NIL ;
   }
   else
   {
      curfrm->next       = newfrm ;
      newfrm->prev       = curfrm ;
   }

   newfrm->form_id  = S_NULL ;
   if (CHK_MASK (flags, HELP_FORM))
      newfrm->flags = HELP_FORM ;
   else
      newfrm->flags =         0 ;
   newfrm->p_prof   =   NULL ;
   newfrm->fst_inf  =   NULL ;
   newfrm->next     =   NULL ;

   s_copy (&(newfrm->form_id), form_id, 0, 0) ;

   return(newfrm);
}
