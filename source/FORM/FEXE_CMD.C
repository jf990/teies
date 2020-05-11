/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FEXE_CMD.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Heidi E. Harting
:DATE.                             July 29, 1987

:INPUT.                line  :  a TEIES string from which the first
                                mark up found will be parsed.

                cmd_tab      :  a pointer to an array of mark up names
                                and their associated functions.

                num_cmds     :  the number of mark ups in the array.

                f_prof       :  a pointer to the structure where the
                                values for mark up qualifiers are
                                stored.

                f_deft       :  a pointer to the structure where the
                                the default values for the mark up
                                qualifiers are stored.

                f_fmat       :  a pointer to a structure used by the
                                Forms Processor format and display
                                routines.

:OUTPUT.               ret   :  a TEIES string that contains the
                                remaining text that needs further
                                parsing.

:INPUT_GLOBALS.        - none -

:OUTPUT_GLOBALS.       - none -

:PURPOSE.
          This routine will parse the first word in the text, assume it
          is a mark up, and if found in the Implemented Mark Up table, it
          will call the routines associated with that mark up, as defined
          in the cmd element in the cmd_tab structure.

          If the word parsed is not matched with a mark up in the
          cmd_tab, the text with the tags will be sent as straight text
          to a format/display routine.

:CALLS.   s_type.c  s_comp.c  s_copy.c  and the routines associated with
          each mark up type (three per mark up) as specified in the
          structure cmd.

:ERRORS.

:LIMITS.

:MODS.    Heidi - Nov 5, 1987 : modified to be passed the pointer to the
          array of key words (CMDS) and the number of key words that have
          associated functions implemented.  These variables have been
          made local so that FEXE_CMD can be used as a general parser.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "form_i.h"
#include "tm.h"
#include "stack.h"

sint  fexe_cmd(str_vec, f_prof, f_fmat, f_deft)

string_vector     str_vec;
struct  f_prof_s *f_prof;
struct  f_fmat_s *f_fmat;
struct  f_prof_s *f_deft;

{
   sint         rc       = 1;
   sint         r        = 0;
   sint         num_cmds = 0;
   sint         i1       = 0;
   sint         i2       = 0;
   sint         i3       = 0;
   boolean      end_mkup = FALSE;
   boolean     *if_cond  = NULL;
   string       garbage  = S_NULL;
   struct form_global * f_global = NIL;
   struct _f_exe_cmd  * cmd_tab  = NIL;





   f_global = (struct form_global *) TM_master->forms ;
   cmd_tab  = (struct _f_exe_cmd  *) f_global->f_cmds ;
   num_cmds =                        f_global->f_ncmds;

   s_upper  (*(str_vec+1), 0, 0);

   if_cond  = (boolean *) stk_get ((STACK *) f_fmat->if_stack );


   if (s_comp (f_prof->end, *(str_vec+1),
               0, s_len (f_prof->end), 'M', &r)==0)
   {
      s_clip (&garbage, str_vec + 1, 0, s_len (f_prof->end) );
      end_mkup = TRUE;
   }

   for ( i1 = 0; i1 < (num_cmds); i1++ )
   {
      if (s_comp (cmd_tab[i1].name, *(str_vec+1), 0, 0, 'M', &r)==0)
      {
         if ( end_mkup &&
              (*if_cond || (cmd_tab[i1].func_typ == FORM_XMP_2) ) &&
              ( (cmd_tab[i1].func_typ == FORM_XMP_3) ||
                (f_prof->all_prse                    )            ) &&
              ( (cmd_tab[i1].func_typ == FORM_XMP_0) ||
                (f_prof->mkp_prse                    )            )    )
         {
            if (cmd_tab[i1].end_fun != NIL )
            {
               (*cmd_tab[i1].end_fun)(f_prof, f_fmat);
               rc = 0;
            }
         } else if ((f_prof->mkp_prse) && (f_prof->all_prse) &&
                   (*if_cond || (cmd_tab[i1].func_typ == FORM_XMP_2)) )
         {
            if (cmd_tab[i1].arg_fun != NIL )
            {
               rc = (*cmd_tab[i1].arg_fun)( str_vec, i1, f_prof,
                                                       f_deft, cmd_tab );
            }
            if (cmd_tab[i1].exe_fun != NIL )
            {
               (*cmd_tab[i1].exe_fun)(f_prof, f_fmat);
               rc = 0;
            }
         }
         break;
      }
   }

   if ( i1 == num_cmds )
   {
      rc = 1;
   }

   s_drop ( & garbage );
   os_memd(   if_cond );

   return(rc);
}
