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

extern byte *colors[];

sint    farg_h(str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{
   string   temp=S_NULL;
   sint     i=0;
   sint     j=0;
   sint     id=0;
   sint     level=0;


   f_prof->h_level = f_deft->h_level;


   if (s_vlen (str_vec) > 1)
   {
      if ( s_type(*(str_vec + 2), 0, 0, 'N') == 0)
      {
         s_stosi(&level,*(str_vec + 2), 0, 0) ;
         f_prof->h_level = level;
      }
      if (s_vlen (str_vec) > 2)
      {
         for (j = 2; j <= s_vlen (str_vec); j++)
         {
            id = f_qual (*(str_vec + j), tags[cmd_num].quals );
            if ( id != 0 )
            {
               s_upper (*(str_vec + j + 1), 0, 0);
               switch (id)
               {
                  case 1:
                     s_copy (&(f_prof->h_id), *(str_vec + j + 1), 0, 0);
                     j++;
                     break;
                  case 2:
                     s_copy (&(f_prof->h_title), *(str_vec+j+1), 0, 0);
                     j++;
                     break;
               }
            }
         }
      }
   }


   return(0);
}
