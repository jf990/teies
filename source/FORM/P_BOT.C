/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_bot                                                  |
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

sint  p_bot   (p_prof)
struct p_prof_s *p_prof ;
{
   wind_dat *body      = NULL ;
   tint      tot_pages =    0 ;


   body = p_prof->body ;

   tot_pages = (tint) (body->w_max_y - body->w_min_y)   /     \
               (tint) (body->win_len + 1.0) ;
                                  /* Set new current Y position        */
   p_prof->w_cur_y =  body->w_max_y - body->win_len -        \
                     (body->win_len + 1.0) * (real) tot_pages ;


   return(0);
}
