/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_eor.c                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :GLOBAL.  name        type     INPUT    desc                     |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. F_parse call this routine after a record has been read |*/
/*|            in and processed.  This is for treatment of <xmp>,     |*/
/*|            <h>, <center>, and other routines which are dependent  |*/
/*|            on how records are stored.                             |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
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

sint f_eor (f_prof,f_fmat)
struct f_prof_s *f_prof;
struct f_fmat_s *f_fmat;
{

                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;
                                  /* If text is in example mode        */
   if(f_prof->all_prse && (f_fmat->cur_x!=f_fmat->org_x+f_fmat->lef_mar))
   {
      f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar ;
      f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
   }

   return(0);
}
