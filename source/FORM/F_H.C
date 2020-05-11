/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_h                                                    |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    November 9, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  None.                                                  |
  |                                                                   |
  |  :PARAM.   None.                                                  |
  |                                                                   |
  |  :PURPOSE. To execute the markup <h#>                             |
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
#include "tm.h"
#include "debug.h"
#include "stack.h"
#include "form_i.h"

sint f_h (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{
   struct  p_prof_s *p_prof = NULL  ; /* Ptr to PM's profile           */
   sint    level            =    0  ; /* Heading level                 */
   tint    lines            =    0  ; /* Current line in form          */
   tint    pages            =    0  ; /* Current page in form          */
   real    down             =  1.0  ; /* How far to go down (WC)       */
   sint    color_f          =    0  ; /* Foreground color              */
   sint    color_b          =    0  ; /* Background color              */
   sint    hi_lite          =    0  ; /* Highlight attributes          */
   sint    font             =    0  ; /* Font information              */
   boolean tof              = FALSE ; /* Boolean am I at TOF?          */
   real    w_max_y          =  0.0  ; /* Maximum Y position of window  */
   real    win_len          =  0.0  ; /* Vertical window length        */

   #D_begin (f_h, FFUN)
                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;

   p_prof = (struct p_prof_s *)
           ((struct form_process *) TM_master->current->forms)->p_prof ;
   level  = f_prof->h_level ;

   #D_show ('H_LEVEL = %d cur_x=%f cur_y=%f status=0x%X',   \
            'level,f_fmat->cur_x,f_fmat->cur_y,f_fmat->status') ;
   #D_str  ('f_prof->h_id','Heading ID') ;
   #D_str  ('f_prof->h_title','Heading Title') ;
                                  /* Close and then open a new sement  */
   gcls_seg () ;
   f_fmat->last_seg += 1 ;
   gcreat_seg  (f_fmat->last_seg) ;
                                  /* Set flag and values in F_FMAT     */
   SET_MASK (f_fmat->status, HEADING) ;
   f_fmat->head_lev = level   ;
                                  /* Make sure cursor is at Left Mar   */
   if (f_fmat->cur_x != f_fmat->org_x + f_fmat->lef_mar)
   {
      f_fmat->cur_x = f_fmat->org_x + f_fmat->lef_mar ;
      f_fmat->cur_y = f_fmat->cur_y - (real) f_fmat->spacing ;
   }
                                  /* Get win len and maximum y of win  */
   if      (CHK_MASK(f_fmat->status, TOP_MAT))
   {
      w_max_y = p_prof->top_mat->w_max_y ;
      win_len = p_prof->top_mat->win_len ;
   }
   else if (CHK_MASK(f_fmat->status, BOT_MAT))
   {
      w_max_y = p_prof->bot_mat->w_max_y ;
      win_len = p_prof->bot_mat->win_len ;
   }
   else
   {
      w_max_y = p_prof->body->w_max_y ;
      win_len = p_prof->body->win_len ;
   }
                                  /* Compute current line and page     */
   lines   = w_max_y - f_fmat->cur_y ;
   pages   =  lines / ((tint)win_len + 1)       ;
   tof     = (lines % ((tint)win_len + 1) == 0) ;

   switch (level)
   {
   case 0:                        /* TOF - CAPS - HP4                  */
         if (!tof)
         {
            f_fmat->cur_y = w_max_y - ((real)pages + 1.0) * \
                            (win_len + 1.0) ;
         }
         SET_MASK (f_fmat->status, UPPERCASE) ;
         hi_lite = INTENSIFY ;
         color_f =         7 ;  /* White     */
         break ;
   case 1:                        /* TOF - CAPS - HP3                  */
         if (!tof)
         {
            f_fmat->cur_y = w_max_y - ((real)pages + 1.0) * \
                            (win_len + 1.0) ;
         }
         SET_MASK (f_fmat->status, UPPERCASE) ;
         hi_lite = INTENSIFY ;
         color_f =         6 ;  /* Yellow    */
         break ;
   case 2:                        /* 4   - CAPS - HP2                  */
         if (!tof)
         {
            f_fmat->cur_y = f_fmat->cur_y - 4.0 * (real)f_fmat->spacing;
         }
         SET_MASK (f_fmat->status, UPPERCASE) ;
         hi_lite = INTENSIFY ;
         color_f =         3 ;  /* Pink      */
         break ;
   case 3:                        /* 3   - CAPS - HP1                  */
         if (!tof)
         {
            f_fmat->cur_y = f_fmat->cur_y - 3.0 * (real)f_fmat->spacing;
         }
         SET_MASK (f_fmat->status, UPPERCASE) ;
         hi_lite = INTENSIFY ;
         color_f =         1 ;  /* Blue      */
         break ;
   case 4:                        /* 2   - HP1                         */
         if (!tof)
         {
            f_fmat->cur_y = f_fmat->cur_y - 2.0 * (real)f_fmat->spacing;
         }
         hi_lite = INTENSIFY ;
         color_f =         5 ;  /* Turquoise */
         break ;
   case 5:                        /* 1   - HP1 - ':' - newline         */
         if (!tof)
         {
            f_fmat->cur_y = f_fmat->cur_y - 1.0 * (real)f_fmat->spacing;
         }
         hi_lite = INTENSIFY ;
         color_f =         5 ;  /* Blue      */
         break ;
   case 6  :                      /* 1   - HP1 - ':'                   */
         if (!tof)
         {
            f_fmat->cur_y = f_fmat->cur_y - 1.0 * (real)f_fmat->spacing;
         }
         hi_lite = INTENSIFY ;
         color_f =         5 ;  /* Turquoise */
         break ;
   default :
         hi_lite = REVVIDEO | INTENSIFY | UNDERLINE | BLINKING ;
         color_f =         2 ;  /* Red       */
         break ;
   }


   gs_shigh  (f_fmat->last_seg, hi_lite) ;
   gs_scolor (f_fmat->last_seg, color_f) ;

/* FOR HELP IN DEBUGGING                                               */
   color_b =    -1 ;
   font    = level ;
/* TEMPORARY DATA FOR HELP IN DEBUGGING                                */

                                  /* Push attributes on the stack      */
   stk_psh ((STACK *)f_fmat->hi_lite, (byte *) & hi_lite) ;
   stk_psh ((STACK *)f_fmat->color_f, (byte *) & color_f) ;
   stk_psh ((STACK *)f_fmat->color_b, (byte *) & color_b) ;
   stk_psh ((STACK *)f_fmat->font   , (byte *) & font   ) ;

   #D_show ('Pushed values : hi_lit=%d fg_col=%d bg_col=%d font=%d', \
            'hi_lite, color_f, color_b, font')

   #D_show ('new_position = (%f, %f)','f_fmat->cur_x,f_fmat->cur_y');

   #D_return (0) ;
}
