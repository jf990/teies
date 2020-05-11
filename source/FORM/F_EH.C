/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    f_eh ()                                                |
  |  <AUTHOR>  Nestor R. Voronka                                      |
  |  <DATE>    November 11, 1987                                      |
  |  <LEVEL>   FORMS.FFUN                                             |
  |  <PARAM>   *f_prof   struct f_prof_s  INPUT                       |
  |                                                                   |
  |  <PARAM>   *f_fmat   struct f_fmat_s  IN/OUT                      |
  |                                                                   |
  |  <PURPOSE> To process the </h> markup.                            |
  |  <OUTPUT>  sint                                                   |
  |  <LIMITS>  The string colon should be set in f_profile,           |
  |            at some future indetermined time.                      |
  |  <MODS>    Sept. 21, 1989   JF    Added spacing after end header  |
  |                                   based on header level.          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "stack.h"
#include "ti.h"
#include "form_i.h"

sint f_eh (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   string colon  = S_NULL ;       /* String containing : for f_render  */
   sint *hi_lite =   NULL ;       /* Highlighting     attribute value  */
   sint *color_f =   NULL ;       /* Foreground color attribute value  */
   sint *color_b =   NULL ;       /* Background color attribute value  */
   sint *font    =   NULL ;       /* Font             attribute value  */


   if (CHK_MASK(f_fmat->status, HEADING))
   {                              /* Clear HEADING and UPPERCASE bits  */
      CLR_MASK (f_fmat->status, HEADING  ) ;
      CLR_MASK (f_fmat->status, UPPERCASE) ;
                                  /* Put ':' into a string             */
      s_init (&colon, ":") ;


      switch( f_fmat->head_lev )
      {

         case  5 :
                                  /* Display ':' and wrap to next line */

            g_ttext (f_fmat->cur_x+1.0, f_fmat->cur_y, colon) ;
            if (f_fmat->cur_x != f_fmat->org_x +f_fmat->lef_mar)
            {
               f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar ;
               if (f_fmat->just != N_JUST)
               {
                  f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
               }

            }
            break;

         case  6 :
                                  /* Display ':' and move 2 spaces     */

            g_ttext (f_fmat->cur_x+1.0, f_fmat->cur_y, colon) ;
            f_fmat->cur_x = f_fmat->cur_x + 3.0 ;
            break;

                                  /* Move to the lef mar & next line   */
         case  1 :
         case  2 :

            f_fmat->cur_y =
                          f_fmat->cur_y - 1.0 * (real) f_fmat->spacing ;

         case  3 :
         case  4 :

            f_fmat->cur_y =
                          f_fmat->cur_y - 1.0 * (real) f_fmat->spacing ;

            if (f_fmat->cur_x != f_fmat->org_x +f_fmat->lef_mar)
            {
               f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar ;
               if (f_fmat->just != N_JUST)
               {
                  f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
               }
            }
            break;


         default :

            break;

      }

      s_drop (&colon) ;

                                  /* Pop attributes from stacks        */
   hi_lite  = (sint *) stk_pop ( (STACK *) f_fmat->hi_lite) ;
   color_f  = (sint *) stk_pop ( (STACK *) f_fmat->color_f) ;
   color_b  = (sint *) stk_pop ( (STACK *) f_fmat->color_b) ;
   font     = (sint *) stk_pop ( (STACK *) f_fmat->font   ) ;




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
   {

      gs_shigh (f_fmat->last_seg, *hi_lite) ;
      os_memd (hi_lite) ;

   }
                                  /* Set foreground color if you can   */
   if (color_f != NULL)
   {

      gs_scolor (f_fmat->last_seg, *color_f) ;
      os_memd (color_f) ;

   }
                                  /* Deallocate gotten memory          */
   os_memd (color_b) ;
   os_memd (font   ) ;


   return(0);
   }
   else
   return(0);
}
