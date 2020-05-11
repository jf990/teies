/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_table.c                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :GLOBAL.  name        type     INPUT    desc                     |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Processes <table> markups                              |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    11_Aug_87   K.J.Walsh   Dummy version written.         |*/
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

sint f_table(f_prof,f_fmat)
struct f_prof_s *f_prof;
struct f_fmat_s *f_fmat;
{
                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;


   return(0);
}
