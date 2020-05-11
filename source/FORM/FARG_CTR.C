/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_CTR.C
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

extern byte *colors[];

sint    farg_ctr( str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{
   sint     place             =   0;
   sint     id                =   0;


   s_copy (&f_prof->ctr_brk, f_deft->ctr_brk, 0, 0);


   id = f_qual (*(str_vec + 2), tags [cmd_num].quals );
   if ( id == 1 )
   {
      s_upper (*(str_vec + 3), 0, 0);
      s_copy (&(f_prof->ctr_brk), *(str_vec + 3), 0, 0);
   }

   return(0);
}
