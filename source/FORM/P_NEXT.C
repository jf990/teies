/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    p_next                                                 |
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

sint  p_next  (p_prof)
struct p_prof_s *p_prof ;
{
   wind_dat *body     = NULL ;


   body = p_prof->body ;
                                  /* Set new current Y position        */
   p_prof->w_cur_y = p_prof->w_cur_y - (body->win_len + 1.0) ;

                                  /* Halt at bottom                    */
   if (p_prof->w_cur_y < body->w_min_y)
   {

      p_bot (p_prof) ;

   }

   return(0);
}
