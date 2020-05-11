/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_right                                                |
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
  |  :CALLS.   p_show_pg                                              |
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

sint  p_right (p_prof)
struct p_prof_s *p_prof ;
{
   wind_dat *body     = NULL ;


   body = p_prof->body ;
                                  /* Set new current X position        */
   p_prof->w_cur_x = p_prof->w_cur_x + body->win_wid ;

                                  /* If past max_x-win_len hold at it  */
   if (p_prof->w_cur_x > body->w_max_x - body->win_wid)
      p_prof->w_cur_x = body->w_max_x - body->win_wid ;


   return(0);
}
