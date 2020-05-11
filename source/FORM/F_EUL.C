/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_EUL.c
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 12, 1988

:INPUT.               f_prof :  pointer to the structure f_prof_s

                      f_fmat :  pointer to the structure f_fmat_s

:OUTPUT.                  rc :  a SINT indicating if there was an <ul>
                                to end.

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
         This routine will call f_li_pop.c to end the list type
         'unordered list'.  If pop_list.c was successful in ending an
         <ul>, this routine will decriment the number of active
         unordered lists.  If f_li_pop.c is not successful, it means that
         an unordered list type is not currently active, and the mark up
         is then sent to be rendered as straight text.

:CALLS.  f_li_pop.c

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

sint  f_eul (f_prof, f_fmat)
struct  f_prof_s *f_prof;
struct  f_fmat_s *f_fmat;

{
    sint    rc=0;
    boolean end_mark=TRUE;


    rc = f_li_pop(f_prof, f_fmat, UNORD_LIST);   /* pop last unordered */

    if ( rc ) {                      /* was an unordered list to end   */
       f_fmat->ul_level -= 1;        /* new current unordered list lev */
       f_fmat->lef_mar = f_fmat->lef_mar - LVL_IND;     /* indent less */
       }

    else {                           /* unordered list not initiated   */
       f_li_err( f_prof, f_fmat, end_mark );/* error, no matching <ul> */
       }

   return(0);
}
