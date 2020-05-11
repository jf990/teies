/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_alarm                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    February 9, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  TM_master->current->p_prof  struct p_prof_s *  IN/OUT  |
  |                                        Ptr to the Presentation    |
  |                                        Manager's profile.         |
  |                                                                   |
  |  :PARAM.   f_prof  struct f_prof_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains default & parsed  |
  |                                        qualifier values.          |
  |                                                                   |
  |  :PARAM.   f_fmat  struct f_fmat_s *   IN/OUT                     |
  |                                        Pointer to structure that  |
  |                                        contains info required to  |
  |                                        render data properly in    |
  |                                        the GKS world.             |
  |                                                                   |
  |  :PURPOSE. To execute the markup <alarm>                          |
  |                                                                   |
  |  :OUTPUT.  Always returns 0.                                      |
  |                                                                   |
  |  :CALLS.   None.                                                  |
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
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "tm.h"
#include "debug.h"
#include "form_i.h"

sint f_alarm (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   struct p_prof_s *p_prof = NIL; /* Pointer to PM's profile           */

                                  /* Get Pointer to the PM's profile & */
                                  /* Set alarm flag to TRUE formthe PM */

   p_prof = (struct p_prof_s *)
           ((struct form_process *) TM_master->current->forms)->p_prof;
   p_prof->alarm = TRUE ;

   return(0);
}
