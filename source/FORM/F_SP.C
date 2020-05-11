/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_space                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    November 24, 1987                                      |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.   None.                                                  |
  |                                                                   |
  |  :PURPOSE. To execute the markup <space>                          |
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

sint f_sp (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   real down = 0.0 ;

                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;

   if (f_fmat->cur_x == f_fmat->lef_mar)
      down = (real) f_prof->sp_amt ;
   else
   {
      f_fmat->cur_x = f_fmat->lef_mar ;
      down = (real) f_prof->sp_amt + 1.0 ;
   }

   f_fmat->cur_y = f_fmat->cur_y - down * (real) f_fmat->spacing ;

   return(0);
}
