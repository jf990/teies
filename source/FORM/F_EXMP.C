/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_exmp                                                 |
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
  |  :PURPOSE. To execute the markup </xmp>                           |
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
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "form_i.h"

sint f_exmp (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{

   f_prof->mkp_prse = TRUE   ;
   f_prof->var_prse = TRUE   ;
   f_fmat->just     = L_JUST ;

   return(0);
}
