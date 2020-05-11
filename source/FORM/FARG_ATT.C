/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_ATT.c
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Stephen Muccione
:DATE.                             July 7, 1989

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

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"

sint    farg_att(str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{


   sint     j                 =   0,
            id                =   0,
            level             =   0;


   s_copy (&(f_prof->att_name), f_deft->att_name , 0, 0);
   s_copy (&(f_prof->att_ref ), f_deft->att_ref  , 0, 0);


   if (s_vlen (str_vec) > 2)
   {
      for (j = 2; j < s_vlen (str_vec); j++)
      {
         id = f_qual (*(str_vec + j), tags [cmd_num].quals );
         if ( id != 0 )
         {
            s_upper (*(str_vec + j + 1), 0, 0);
            switch (id)
            {
               case 1:
                  s_copy (&(f_prof->att_name), *(str_vec + j + 1), 0, 0);
                  j++;
                  break;
               case 2:
                  s_copy (&(f_prof->att_ref ), *(str_vec + j + 1), 0, 0);
                  j++;
                  break;
            }
         }
      }
   }


   return(0);
}
