/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_p                                                    |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    October 12, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  f_fmat->cur_x   real  OUTPUT  Current X in form        |
  |  :GLOBAL.  f_fmat->cur_y   real  IN/OUT  Current Y in form        |
  |  :GLOBAL.  f_fmat->lef_mar real   INPUT  Current Left margin      |
  |  :GLOBAL.  f_fmat->spacing tint   INPUT  Current Spacing in form  |
  |                                                                   |
  |  :PARAM.   None.                                                  |
  |                                                                   |
  |  :PURPOSE. To execute the markup <p>                              |
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
#include "debug.h"
#include "form_i.h"

sint f_p (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   real down = 1.0 ;

                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;
                                  /* If 'cursor' is already at margin  */
   if (f_fmat->cur_x == f_fmat->org_x + f_fmat->lef_mar)
      down = 1.0 ;
   else
   {
      down = 2.0 ;
      f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar ;
   }
                                  /* Move down                         */
   f_fmat->cur_y = f_fmat->cur_y - down * (real) f_fmat->spacing ;
                                  /* Close current segment             */
   gcls_seg () ;
                                  /* Increment segment counter         */
   f_fmat->last_seg += 1 ;
                                  /* Create new segment                */
   gcreat_seg (f_fmat->last_seg) ;

   return(0);
}
