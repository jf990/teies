/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FEND_CMD.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Heidi E. Harting
:DATE.                             Oct 14, 1987

:INPUT.                line  :  a TEIES string containing text that will
                                be left untouched by this routine.

:OUTPUT.               line  :  the same TEIES string that was pasted
                                into this routine.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -

:PURPOSE.
         This routine does nothing, but is needed to fullfill the pattern
         of a qualifier parsing routine for every mark up.

:CALLS.  - none -

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "str.h"
#include "debug.h"
#include "form_i.h"

sint   fend_cmd(line,f_prof,f_deft)
string line;
struct f_prof_s *f_prof;
struct f_prof_s *f_deft;

{

    #D_begin(FEND_CMD,PARSER)
    #D_return(0);
}
