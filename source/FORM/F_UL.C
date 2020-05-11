/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_UL.C
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 12, 1988

:INPUT.               f_prof :  pointer to the structure f_prof_s

                      f_fmat :  pointer to the structure f_fmat_s

:OUTPUT.                        - none -

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
         This routine will fill in a list-information structure, which
         will then be placed on the top of the list stack as the current
         list type.  Until another list type is specified, all <li>
         items will be considered to apply to an unordered list.  This
         routine will also incriment the number of unordered lists that
         are presently active.

:CALLS.  stk_psh.c  osmemd.c

:ERRORS.

:LIMITS.

:MODS.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "tm.h"
#include "stack.h"
#include "debug.h"
#include "form_i.h"

sint  f_ul (f_prof, f_fmat)
struct  f_prof_s *f_prof;
struct  f_fmat_s *f_fmat;

{
    struct  f_lst_s  list;

                                  /* Clear Heading Markup if active    */
   if (CHK_MASK (f_fmat->status, HEADING))
      f_eh (f_prof, f_fmat) ;

    f_fmat->ul_level += 1;           /* new current unordered list lev */

    list.type  = UNORD_LIST;         /* code for unordered list        */
    list.level = f_fmat->ul_level;   /* gives current level of lists   */
    list.seqno = 0;                  /* set sequence no. to 0          */

    stk_psh( (STACK *) f_fmat->list_stk, (byte *) & list );

    f_fmat->lef_mar += LVL_IND;       /* indent new level further      */

   return(0);
}
