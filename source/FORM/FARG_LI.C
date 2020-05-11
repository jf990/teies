/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_H.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Heidi E. Harting
:DATE.                             Sept 7, 1987

:INPUT.                line  :  a TEIES string containing the text
                                needing parsing.

:OUTPUT.               line  :  a TEIES string that has been updated
                                pasted the mark-up and its qualifiers.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -
:PURPOSE.
          This routine will parse the highlight code corresponding to
          the font type to be used, the color of the highlighting,
          and store the parsed qualifiers in the F_PROF structure
          as defined in FORMP_I.H.

:CALLS.   s_copy.c   s_stosi.c   s_type.c

:ERRORS.

:LIMITS.

:MODS.    Heidi:  This routine works as of September 3, 1987

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"

sint    farg_h(str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{


   return(0);
}
