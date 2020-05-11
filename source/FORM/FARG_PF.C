/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_PF.C
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

          PF markup can redefine existing keys or create user-defined
          keydefs.

:CALLS.   s_copy.c   s_stosi.c   s_type.c
</DOC>
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"





sint    farg_pf(str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{
   string   temp=S_NULL;
   sint     key=0;
   sint     i=0;
   sint     j=0;
   sint     rc=0;
   sint     id=0;
   sint     R=0;
   struct   p_prof_s *p_prof=NIL;


   p_prof   = (struct p_prof_s *)
             ((struct form_process *)TM_master->current->forms)->p_prof;

   if (s_vlen (str_vec) > 2)
   {
      for (j = 2; j <= s_vlen (str_vec); j += 2)
      {
         if ( s_type(*(str_vec + j), 0, 0, 'N') == 0)
         {
            s_stosi(&key,*(str_vec + 2), 0, 0) ;
            if ( (key > F_MIN_FUNCTION_KEY_VALUE)
              && (key < F_MAX_FUNCTION_KEY_VALUE) )
            {
            }
            else
            {


            }
         }
      }
   }
   return(0);
}
