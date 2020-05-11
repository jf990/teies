/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_fig.c                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :GLOBAL.  name        type     INPUT    desc                     |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Processes <fig> markups                                |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    19_Oct_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"
#include "form_i.h"

sint f_fig(f_prof,f_fmat)
struct f_prof_s *f_prof;
struct f_fmat_s *f_fmat;
{
                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;


   return(0);
}
