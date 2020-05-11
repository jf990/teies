/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_beg                                                  |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    September 30, 1987                                     |
  |                                                                   |
  |  :LEVEL.   FORMS.PM                                               |
  |                                                                   |
  |  :GLOBAL.  None.                                                  |
  |                                                                   |
  |  :PARAM.   formid    string             INPUT                     |
  |               Form Identification string                          |
  |                                                                   |
  |  :PARAM.  *p_prof    struct p_prof_s    INPUT                     |
  |               Presentation Managers profile                       |
  |                                                                   |
  |  :PURPOSE. To do whatever is neccessary before the Presentation   |
  |            Manager starts displaying.                             |
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
#include "debug.h"
#include "form_i.h"

sint p_beg (form_id, p_prof)
string form_id ;
struct p_prof_s *p_prof ;
{
   sint      curr_seg = 0 ;       /* Current segment being redrawn     */
   sint      frst_seg = 0 ;       /* First   segment being redrawn     */
   sint      last_seg = 0 ;       /* Last    segment being redrawn     */
   wind_dat *body     = NULL ;
   wind_dat *top_mat  = NULL ;
   wind_dat *bot_mat  = NULL ;


   body    = p_prof->body    ;
   top_mat = p_prof->top_mat ;
   bot_mat = p_prof->bot_mat ;

   p_prof->leave = FALSE ;
                                  /* Set viewport of body              */
   gs_view (body->trans_nm,                 \
            body->v_min_x  , body->v_max_x, \
            body->v_min_y  , body->v_max_y) ;

                                  /* Set viewport and window of topmat */
   if (top_mat != NULL)
   {
      gs_view (top_mat->trans_nm,                    \
               top_mat->v_min_x  , top_mat->v_max_x, \
               top_mat->v_min_y  , top_mat->v_max_y) ;
      gs_wind (top_mat->trans_nm,                    \
               top_mat->w_min_x  , top_mat->w_max_x, \
               top_mat->w_min_y  , top_mat->w_max_y) ;
   }
                                  /* Set viewport and window of botmat */
   if (bot_mat != NULL)
   {
      gs_view (bot_mat->trans_nm,                    \
               bot_mat->v_min_x  , bot_mat->v_max_x, \
               bot_mat->v_min_y  , bot_mat->v_max_y) ;
      gs_wind (bot_mat->trans_nm,                   \
               bot_mat->w_min_x  , bot_mat->w_max_x, \
               bot_mat->w_min_y  , bot_mat->w_max_y) ;
   }

   return(0);
}
