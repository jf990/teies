/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_FLD.c
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Stephen Muccione
:DATE.                             June 22, 1989

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
#include "ti.h"
#include "tm.h"
#include "form_i.h"

extern byte *justifys[];

sint    farg_fld (str_vec, cmd_num, f_prof, f_deft, tags )

string_vector     str_vec;
sint              cmd_num;
struct  f_prof_s *f_prof;
struct  f_prof_s *f_deft;
struct _f_exe_cmd * tags;

{
   string   just=S_NULL;
   sint     i=0;
   sint     j=0;
   sint     id=0;
   sint     R=0;
   sint     rc=0;
   sint     level=0;


   s_copy(&f_prof->io_otype,f_deft->io_otype,0,0);
   s_copy(&f_prof->io_id,f_deft->io_id,0,0);
   s_copy(&f_prof->io_name,f_deft->io_name,0,0);
   f_prof->io_var = f_deft->io_var;
   s_copy(&f_prof->io_index,f_deft->io_index,0,0);
   s_copy(&f_prof->io_prompt,f_deft->io_prompt,0,0);
   f_prof->io_io = f_deft->io_io;
   f_prof->io_echo = f_deft->io_echo ;
   f_prof->io_len = f_deft->io_len;
   f_prof->io_pad = f_deft->io_pad;
   f_prof->io_just = f_deft->io_just;
   s_copy (&f_prof->io_def,f_deft->io_def,0,0);


   if (s_vlen (str_vec) > 2)
   {
      for (j = 2; j <= s_vlen (str_vec); j++)
      {
         id = f_qual (*(str_vec + j), tags [cmd_num].quals );
         if ( id != 0 )
         {
            s_upper (*(str_vec + j + 1), 0, 0);
            switch (id)
            {
               case 1:
                  s_copy (&f_prof->io_name, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 2:
                  s_copy (&f_prof->io_id, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 3:
                  f_prof->io_var = TRUE;
                  break;
               case 4:
                  f_prof->io_io = FLD_INPUT;
                  break;
               case 5:
                  f_prof->io_io = FLD_OUTPUT;
                  break;
               case 6:
                  s_stosi (&f_prof->io_len, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 7:
                  s_upper (*(str_vec + j + 1), 0, 0);
                  for (i = 0; i < 3; i++ )
                  {
                     s_init (&just, justifys[i]);
                     rc=s_comp( *(str_vec + j+1), just, 0, 0, 'M', &R);
                     if ( rc == 0 )
                     {
                        f_prof->io_just = i + 1;
                        break;
                     }
                  }
                  s_drop (&just);
                  j++;
                  break;
               case 8:
                  s_copy (&f_prof->io_def, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 9:
                  s_stob (&f_prof->io_pad, *(str_vec+j+1), 1);
                  j++;
                  break;
               case 10:
                  s_copy (&f_prof->io_prompt, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 11:
                  s_copy (&f_prof->io_help, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 12:
                  f_prof->io_echo = NOECHO;
                  break;
               case 13:
                  s_copy (&f_prof->io_otype, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 14:
                  s_copy (&f_prof->io_index, *(str_vec+j+1), 0, 0);
                  j++;
                  break;
               case 15:
                  f_prof->io_io = FLD_REPLACE;
                  break;
               case 16:
                  /* not implemented yet */
                  break;
            }
         }
      }
   }

   s_drop (&just);

   return(0);
}
