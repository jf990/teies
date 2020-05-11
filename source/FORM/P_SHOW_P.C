/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_show_p                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    September 3, 1987                                      |
  |                                                                   |
  |  :LEVEL.   FORMS.PM                                               |
  |                                                                   |
  |  :GLOBAL.                                                         |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. Clear the screen, redraw the viewport of the form.     |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "ti.h"
#include "tm.h"
#include "form_i.h"

sint  p_show_p (p_prof)
struct p_prof_s *p_prof ;
{
   wind_dat *body     = NULL ;    /* Copied pointer to body  of form   */
   wind_dat *top_mat  = NULL ;    /* Copied pointer to topm  of form   */
   wind_dat *bot_mat  = NULL ;    /* Copied pointer to botm  of form   */
   sint      wk_id    =    0 ;    /* Copied workstation ID             */
   sint      frst_seg =    0 ;    /* Copied first   segment number     */
   sint      last_seg =    0 ;    /* Copied last    segment number     */
   sint      curr_seg =    0 ;    /* Counter with current segment numb */


   wk_id    = p_prof->wk_id   ;
   body     = p_prof->body    ;
   top_mat  = p_prof->top_mat ;
   bot_mat  = p_prof->bot_mat ;
   frst_seg = body->frst_seg  ;
   last_seg = body->last_seg  ;


                                  /* Clear and redraw topmat viewport  */
   if (  top_mat != NIL  )
   {


      gclr_vi (p_prof->wk_id, top_mat->trans_nm) ;
                                  /* Get first and last segment number */
      frst_seg = top_mat->frst_seg ;
      last_seg = top_mat->last_seg ;
                                  /* Select this as the cur transform  */
      g_seltrn (top_mat->trans_nm) ;
                                  /* Redraw first to last segments     */

      for (curr_seg = frst_seg ; curr_seg <= last_seg ; curr_seg++)
      {

         g_rdwseg (p_prof->wk_id, curr_seg) ;

      }
   }
                                  /* Clear and redraw botmat viewport  */
   if (  bot_mat != NIL  )
   {


      gclr_vi (p_prof->wk_id, bot_mat->trans_nm) ;
                                  /* Get first and last segment number */
      frst_seg = bot_mat->frst_seg ;
      last_seg = bot_mat->last_seg ;
                                  /* Select this as the cur transform  */
      g_seltrn (bot_mat->trans_nm) ;
                                  /* Redraw first to last segments     */


      for (curr_seg = frst_seg ; curr_seg <= last_seg ; curr_seg++)
      {

         g_rdwseg (p_prof->wk_id, curr_seg) ;

      }
   }
                                  /* Select body as the cur transform  */


   g_seltrn (body->trans_nm) ;
                                  /* Define the window in the world    */
                                  /* that is to be mapped to the vwprt */
   gs_wind (body->trans_nm,
            p_prof->w_cur_x, p_prof->w_cur_x + body->win_wid    ,
            p_prof->w_cur_y, p_prof->w_cur_y + body->win_len     ) ;

                                  /* Clear viewport of body            */
   gclr_vi  (p_prof->wk_id, body->trans_nm) ;
                                  /* Get first and last segment number */
   frst_seg = body->frst_seg ;
   last_seg = body->last_seg ;
                                  /* Redraw all segments in body       */


   for (curr_seg = frst_seg ; curr_seg <= last_seg ; curr_seg++)
   {

      g_rdwseg (wk_id, curr_seg) ;

   }

   return(0);
}
