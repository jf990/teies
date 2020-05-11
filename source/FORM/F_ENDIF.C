/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_hp ()                                                |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    January 29, 1988                                       |
  |                                                                   |
  |  :LEVEL.   FORMS.FFUN                                             |
  |                                                                   |
  |  :GLOBAL.  name        type      OUTPUT  description              |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. purpose                                                |
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
#include "tm.h"
#include "stack.h"
#include "debug.h"
#include "form_i.h"

sint f_endif (f_prof, f_fmat)
struct f_prof_s *f_prof ;
struct f_fmat_s *f_fmat ;
{

   boolean  *tmp_bool = NIL;


   if (stk_qry ((STACK *) f_fmat->if_stack) > 1)
   {
      tmp_bool  = (boolean *) stk_pop ((STACK *) f_fmat->if_stack );
   }

   return(0);
}
