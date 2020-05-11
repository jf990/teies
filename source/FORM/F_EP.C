/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_ep ()                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    October 26, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  None used.                                             |
  |                                                                   |
  |  :PARAM.   None used.                                             |
  |                                                                   |
  |  :PURPOSE. To process the </p> markup.                            |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
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

sint f_ep (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   real down = 0.0 ;


   if (f_fmat->cur_x == f_fmat->lef_mar)
      down = 0.0 ;
   else
   {
      down = 1.0 ;
      f_fmat->cur_x = f_fmat->lef_mar ;
   }
                                  /* Move down                         */
   f_fmat->cur_y = f_fmat->cur_y - down * (real) f_fmat->spacing ;

   return(0);
}
