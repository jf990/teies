/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_XMP.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Stephen Muccione
:DATE.                             July 24, 1989

:INPUT.                line  :  a TEIES string containing the text
                                needing parsing.

:OUTPUT.               line  :  a TEIES string that has been updated
                                pasted the mark-up and its qualifiers.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -
:PURPOSE.

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

sint    farg_xmp(str_vec, cmd_num, f_prof, f_deft, tags )

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



   f_prof->mkp_prse = FALSE;
   f_prof->var_prse = FALSE;

   if (s_vlen (str_vec) > 1)
   {
      for (j = 2; j <= s_vlen (str_vec); j++)
      {
         id = f_qual (*(str_vec + j), tags [cmd_num].quals );
         if ( id != 0 )
         {
            switch (id)
            {
               case 1:
                  f_prof->mkp_prse = TRUE;
                  break;
               case 2:
                  f_prof->var_prse = TRUE;
                  break;
            }
         }
      }
   }

   return(0);
}
