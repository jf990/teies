/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_HP.C
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

extern byte *units [];

sint    farg_sp(str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{
   string   temp=S_NULL;
   string   unit=S_NULL;
   sint     i=0;
   sint     j=0;
   sint     level=0;
   sint     rc=0;
   sint     id=0;
   sint      R=0;



   f_prof->sp_amt = f_deft->sp_amt;
   s_copy (&f_prof->sp_units, f_deft->sp_units, 0, 0);

   if (s_vlen (str_vec) > 1)
   {
      if ( s_type(*(str_vec + 2), 0, 0, 'N') == 0)
      {
         s_stosi(&level,*(str_vec + 2), 0, 0) ;
         f_prof->sp_amt = level;
      }
      if (s_vlen (str_vec) > 2)
      {
         s_upper (*(str_vec + 3), 0, 0);
         for (i=0; i < 10; i++ )
         {
            s_init (&unit, units[i]);
            rc=s_comp( *(str_vec + 3), unit, 0, 0, 'M', &R);
            if ( rc == 0 )
            {
               s_copy (&(f_prof->sp_units), unit, 0, 0);
               break;
            }
            s_drop (&unit);
         }
      }
   }
   return(0);
}
