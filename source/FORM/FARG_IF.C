/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_H.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Stephen Muccione
:DATE.                             July 118, 1989

:INPUT.                line  :  a TEIES string containing the text
                                needing parsing.

:OUTPUT.               line  :  a TEIES string that has been updated
                                pasted the mark-up and its qualifiers.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -
:PURPOSE.
          This routine will parse the if markup

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
#include "stack.h"
#include "form_i.h"

sint    farg_if (str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{
   string   null_str = S_NULL;
   sint     i     = 0;
   sint     rc    = 0;
   sint     r     = 0;
   sint     j     = 0;
   sint     id    = 0;
   sint     level = 0;
   OCD     *ocd1  = NULL;
   OCD     *ocd2  = NULL;
   boolean  tmp_bool = FALSE;
   struct   f_fmat_s *f_fmat=NIL;


   f_fmat  = (struct f_fmat_s *)
            ((struct form_process *)TM_master->current->forms)->f_fmat;

   if (s_vlen (str_vec) == 3)
   {
      s_put ( *(str_vec + 2), '\0', s_len ( *(str_vec+2) ) + 1 );
      s_put ( *(str_vec + 3), '\0', s_len ( *(str_vec+3) ) + 1 );
      ocd_new (&ocd1, s_buffer (*(str_vec + 2)) );
      s_init  (&null_str,"NULL");
      rc = s_comp  (*(str_vec + 3),null_str, 0, 0, 'M', &r);
      if (rc == 0)
      {
         tmp_bool = obj_isnull (f_fmat->ocb, ocd1);
         stk_psh ( (STACK *) f_fmat->if_stack,(byte *) &tmp_bool );
      } else
      {
         ocd_new (&ocd2, s_buffer (*(str_vec + 3)) );
         tmp_bool = obj_comp ( f_fmat->ocb, ocd1, f_fmat->ocb, ocd2 );
         stk_psh ( (STACK *) f_fmat->if_stack,(byte *) &tmp_bool );
      }
   }
   return(0);
}
