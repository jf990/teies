/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_home                                                 |
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
  |  :PURPOSE. purpose                                                |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   p_show_p                                               |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "form_i.h"

sint  p_home  (p_prof)
struct p_prof_s *p_prof ;
{
   wind_dat *body = NULL ;        /* Data on the Forms body window     */


   body = p_prof->body ;
                                  /* Set new current X position        */
   p_prof->w_cur_x = body->w_min_x ;
                                  /* Set new current Y position        */
   p_prof->w_cur_y = body->w_max_y - body->win_len ;


   return(0);
}
