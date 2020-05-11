/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_ESL.c
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 12, 1988

:INPUT.               f_prof :  pointer to the structure f_prof_s

                      f_fmat :  pointer to the structure f_fmat_s

:OUTPUT.                  rc :  a SINT indicating if there was an <sl>
                                to end.

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
         This routine will call pop_list.c to end the list type 'simple
         list'.  If pop_list.c was successful in ending an <sl>, this
         routine will decriment the number of active simple lists.  If
         pop_list.c is not successful, it means that a simple list type
         is not currently active, and the mark up is then sent to be
         rendered as straight text.

:CALLS.  pop_list.c

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

sint  f_esl (f_prof, f_fmat)
struct  f_prof_s *f_prof;
struct  f_fmat_s *f_fmat;

{
    sint    rc=0;
    boolean end_mark=TRUE;


    rc = f_li_pop(f_prof, f_fmat,SIMP_LIST);   /* pop last simple list */

    if ( rc ) {                      /* was an simple list to end      */
       f_fmat->sl_level -= 1;        /* new current simple list level  */
       f_fmat->lef_mar = f_fmat->lef_mar - LVL_IND;     /* indent less */
       }

    else {                           /* simple list not initiated      */
       f_li_err( f_prof, f_fmat, end_mark );/* error, no matching <sl> */
       }

   return(0);
}
