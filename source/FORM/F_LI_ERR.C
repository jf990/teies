/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_ERR.C
:LEVEL.                            FORMS.FFUN
:AUTHOR.                           Heidi E. Harting
:DATE.                             Jan 14, 1988

:INPUT.               f_prof :  pointer to structure f_prof_s

                      f_fmat :  pointer to structure f_fmat_s

                    end_mark :  flag indicating if error was with an
                                end mark up or a beginning mark up.

:OUTPUT.                        - none -

:INPUT_GLOBALS.                 - none -

:OUTPUT_GLOBALS.                - none -

:PURPOSE.
          This routine will concatinate the necessary mark up tags, and
          the actual mark up.  It will then send that text to f_render
          to be displayed in the form as straight text.

:CALLS.   s_cat.c  s_copy.c  f_render.c

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
#include "bse.h"
#include "form_i.h"

sint f_li_err( f_prof, f_fmat, end_mark )
struct  f_prof_s  *f_prof;
struct  f_fmat_s  *f_fmat;
boolean end_mark;

{
    string  line=S_NULL,  temp=S_NULL, prefix=S_NULL;


    if ( end_mark ) {                       /* error on an end mark up */
       s_cat( &prefix, f_prof->open, f_prof->end, 0, 0 );
       }
    else {                              /* beginning or single mark up */
       s_copy( &prefix, f_prof->open, 0, 0 );
       }
    s_cat( &temp, f_prof->mark_up, f_prof->cls, 0, 0 );
    s_cat( &line, prefix, temp, 0, 0 );
    f_render( f_fmat, line, F_RENDER_NCR, NIL ) ;

   return(1);
}
