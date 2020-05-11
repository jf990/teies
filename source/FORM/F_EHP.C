/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_ehp ()                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 29, 1988                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  name        type      OUTPUT  description              |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. purpose                                                |
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
#include "ti.h"
#include "tm.h"
#include "stack.h"
#include "debug.h"
#include "form_i.h"

sint f_ehp (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   sint *hi_lite = NULL ;         /* Highlighting     attribute value  */
   sint *color_f = NULL ;         /* Foreground color attribute value  */
   sint *color_b = NULL ;         /* Background color attribute value  */
   sint *font    = NULL ;         /* Font             attribute value  */

                                  /* Pop attributes from stacks        */
   hi_lite  = (sint *) stk_pop ((STACK *)f_fmat->hi_lite) ;
   color_f  = (sint *) stk_pop ((STACK *)f_fmat->color_f) ;
   color_b  = (sint *) stk_pop ((STACK *)f_fmat->color_b) ;
   font     = (sint *) stk_pop ((STACK *)f_fmat->font   ) ;




                                  /* Deallocate pop'ed memory          */
   os_memd (hi_lite) ;
   os_memd (color_f) ;
   os_memd (color_b) ;
   os_memd (font   ) ;

                                  /* Get attributes from stacks        */
   hi_lite  = (sint *) stk_get ((STACK *)f_fmat->hi_lite) ;
   color_f  = (sint *) stk_get ((STACK *)f_fmat->color_f) ;
   color_b  = (sint *) stk_get ((STACK *)f_fmat->color_b) ;
   font     = (sint *) stk_get ((STACK *)f_fmat->font   ) ;



                                  /* Close and then open a new segment */
   gcls_seg () ;
   f_fmat->last_seg = f_fmat->last_seg + 1 ;
   gcreat_seg (f_fmat->last_seg) ;
                                  /* Set highlighting if you can       */
   if (hi_lite != NULL)
      gs_shigh (f_fmat->last_seg, *hi_lite) ;
                                  /* Set foreground color if you can   */
   if (color_f != NULL)
      gs_scolor (f_fmat->last_seg, *color_f) ;
                                  /* Deallocate gotten memory          */
   os_memd (hi_lite) ;
   os_memd (color_f) ;
   os_memd (color_b) ;
   os_memd (font   ) ;

   return(0);
}
