/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_EOL.c
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 12, 1988

:INPUT.               f_prof :  pointer to the structure f_prof_s

                      f_fmat :  pointer to the structure f_fmat_s

:OUTPUT.                  rc :  a SINT indicating if there was an <ol>
                                to end.

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
         This routine will call pop_list.c to end the list type 'ordered
         list'.  If pop_list.c was successful in ending an <ol>, this
         routine will decriment the number of active ordered lists.  If
         pop_list.c is not successful, it means that an ordered list type
         is not currently active, and the mark up is then sent to be
         rendered as straight text.

:CALLS.  pop_list.c  f_render.c

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
#include "debug.h"
#include "form_i.h"

sint  f_eol (f_prof, f_fmat)
struct  f_prof_s *f_prof;
struct  f_fmat_s *f_fmat;

{
    sint    rc=0;
    boolean end_mark=TRUE;


    rc = f_li_pop(f_prof, f_fmat, ORD_LIST);  /* pop last ordered list */

    if ( rc ) {                        /* was an ordered list to end   */
       f_fmat->ol_level -= 1;          /* new current ordered list lev */
       f_fmat->lef_mar = f_fmat->lef_mar - LVL_IND;     /* indent less */
       }

    else {                             /* ordered list not initiated   */
       f_li_err( f_prof, f_fmat, end_mark );/* error, no matching <ol> */
       }

   return(0);
}
