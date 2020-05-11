/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_getorg                                               |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 10, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  name        type      OUTPUT  description              |
  |                                                                   |
  |  :PARAM.   *org_x      real      OUTPUT  X-coordinate (WC) of     |
  |                                          where to render the      |
  |                                          parsed form.             |
  |                                                                   |
  |  :PARAM.   *org_y      real      OUTPUT  Y-coordinate (WC) of     |
  |                                          where to render the      |
  |                                          parsed form.             |
  |                                                                   |
  |  :PARAM.   *last_seg   sint      OUTPUT  Number of segment used.  |
  |                                                                   |
  |  :PURPOSE. This routine returns the coordinates where a new form  |
  |            may be rendered and the last segment number used.      |
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
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"


sint f_getorg (org_x, org_y, last_seg)
real *org_x    ;
real *org_y    ;
sint *last_seg ;
{
   struct f_gks_s *f_world = NULL ;



   f_world = (struct f_gks_s *)
                     ((struct form_global *) TM_master->forms)->f_world ;

   if (f_world == NIL)
   {
      f_world = (struct f_gks_s *) os_mema (sizeof (struct f_gks_s)) ;
      f_world->last_seg =  0   ;
      f_world->bot_y    =  0.0 ;
      ((struct form_global *)TM_master->forms)->f_world = (byte *) f_world;
   }

   *org_x    =                     0.0 ;
   *org_y    = f_world->bot_y    - 1.0 ;
   *last_seg = f_world->last_seg + 1   ;


   return(0);
}
