/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_left                                                 |
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

sint  p_left  (p_prof)
struct p_prof_s *p_prof ;
{
   wind_dat *body     = NULL ;


   body = p_prof->body ;
                                  /* Set new current X position        */
   p_prof->w_cur_x = p_prof->w_cur_x - body->win_wid ;

                                  /* If past min_x hold at min_x       */
   if (p_prof->w_cur_x < body->w_min_x)
      p_prof->w_cur_x = body->w_min_x ;


   return(0);
}
