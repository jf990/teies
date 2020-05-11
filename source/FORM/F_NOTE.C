/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    f_fn.c                                                 |*/
/*|                                                                   |*/
/*|  :LEVEL.   FORMS.FFUN                                             |*/
/*|                                                                   |*/
/*|  :GLOBAL.  name        type     INPUT    desc                     |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Processes <note> makups                                |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    30_Nov_87   K.J.Walsh   Dummy version written.         |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"
#include "form_i.h"

sint f_note(f_prof,f_fmat)
struct f_prof_s *f_prof;
struct f_fmat_s *f_fmat;
{
   #D_begin (f_note, FFUN)

   #D_say('DUMMY VERSION CALLED');

   #D_return(0);
}
