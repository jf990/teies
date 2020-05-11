/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_ecent                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    November 23, 1987                                      |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  f_fmat->just    real  OUTPUT  Current Justification    |
  |                                                                   |
  |  :PARAM.   None.                                                  |
  |                                                                   |
  |  :PURPOSE. To execute the markup </cent>                          |
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
#include "debug.h"
#include "form_i.h"

sint f_ectr (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{

   if (CHK_MASK (f_fmat->status, F_CENTER))
   {

      CLR_MASK  (f_fmat->status, F_CENTER) ;
      if( f_prof->mkp_prse || f_prof->var_prse )
      {

         f_fmat->just = N_JUST ;

      }
      else
      {

         f_fmat->just = L_JUST ;

      }
   }


   return(0);
}
