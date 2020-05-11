/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_erem                                                 |
  |                                                                   |
  |  :AUTHOR.  Stephen Muccione                                       |
  |                                                                   |
  |  :DATE.    August 10, 1989                                        |
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

sint f_erem (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{

   f_prof->all_prse = TRUE;

   return(0);
}
