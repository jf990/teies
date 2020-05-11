/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_hp ()                                                |
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

sint f_hp (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   sint hi_lite = 0 ;             /* Highlighting     attribute value  */
   sint color_f = 0 ;             /* Foreground color attribute value  */
   sint color_b = 0 ;             /* Background color attribute value  */
   sint font    = 0 ;             /* Font             attribute value  */


   switch (f_prof->hp_level)
   {
   case 1:
      hi_lite = INTENSIFY ;
      color_f =         7 ;       /* White     */
      color_b =         0 ;
      font    =         0 ;
      break ;
   case 2:
      hi_lite = INTENSIFY ;
      color_f =         6 ;       /* Yellow    */
      color_b =         0 ;
      font    =         0 ;
      break ;
   case 3:
      hi_lite = INTENSIFY ;
      color_f =         3 ;       /* Pink      */
      color_b =         0 ;
      font    =         0 ;
      break ;
   case 4:
      hi_lite = INTENSIFY ;
      color_f =         1 ;       /* Blue      */
      color_b =         0 ;
      font    =         0 ;
      break ;
   case 5:
      hi_lite = INTENSIFY ;
      color_f =         5 ;       /* Turquoise */
      color_b =         0 ;
      font    =         0 ;
      break ;
   default:
      hi_lite = INTENSIFY ;
      color_f =         2 ;       /* Red       */
      color_b =         0 ;
      font    =         0 ;
      break ;
   }

/* TEMPORARY tweaks to help in debugging of the HP stacks              */
   color_b = f_prof->hp_color ;
   font    = f_prof->hp_level ;
/* TEMPORARY tweaks to help in debugging of the HP stacks              */

                                  /* Push attributes on the stack      */
   stk_psh ((STACK *) f_fmat->hi_lite, (byte *) & hi_lite) ;
   stk_psh ((STACK *) f_fmat->color_f, (byte *) & color_f) ;
   stk_psh ((STACK *) f_fmat->color_b, (byte *) & color_b) ;
   stk_psh ((STACK *) f_fmat->font   , (byte *) & font   ) ;


                                  /* Close and then open a new segment */
   gcls_seg () ;
   f_fmat->last_seg = f_fmat->last_seg + 1 ;
   gcreat_seg (f_fmat->last_seg) ;
                                  /* Set attributes                    */
   gs_shigh  (f_fmat->last_seg, hi_lite) ;
   gs_scolor (f_fmat->last_seg, color_f) ;

   return(0);
}
