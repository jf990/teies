/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FARG_VAR.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Heidi E. Harting
:DATE.                             October 8, 1987

:INPUT.                line  :  a TEIES string to be searched for any
                                embedded variable names that need
                                replacing.

:OUTPUT.                     :

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -
:PURPOSE.
          This routine will determine if there are any names of variables
          that need parsing out.  If yes, the variable name(s) are passed
          to F_SFIELD.C for later replacement with the correct
          information.  The plain text is sent to the F_RENDER.C routine,
          which will later display it on the screen.

:CALLS.   s_copy.c   s_first.c   s_init.c   f_render.c   f_sfield.c

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "bse.h"
#include "form_i.h"


string farg_var(line,f_prof,f_fmat)
string line;
struct f_prof_s *f_prof;
struct f_fmat_s *f_fmat;
{
   string  temp=S_NULL, var=S_NULL, blank=S_NULL, line_c=S_NULL;
   sint    s_place, e_place, b_place, len;


   s_copy(&line_c,line,0,0);              /* make a copy to manipulate */
   s_init(&blank," ");                                  /* blank space */

   while ( s_len(line_c) > 0 ) {                /* more text to search */
      s_place = 0;                                       /* initialize */
      s_place=s_first(f_prof->bvar,line_c,0,0);/* is there a variable? */
      if ( s_place > 0 ) {                      /* possibly a variable */
         b_place = s_first(blank,line_c,s_place,0);  /* 1st sp after & */
         e_place=s_first(f_prof->evar,line_c,s_place,0);/* 1st end-tag */
         if ( b_place == 0 )                 /* no more spaces on line */
            b_place = e_place + 1;
         if ( e_place < b_place && e_place > 0 ) { /* it is a variable */
            if ( s_place > 1 ) {    /* variable not first word in text */
               s_place--;                  /* don't include the & sign */
               s_copy(&temp,line_c,0,s_place);
               f_render( f_fmat, temp, B_PRINTABLE, NIL ) ;
               s_place++;
               }
            e_place--;                          /* don't include the ; */
            s_place = s_place + 1;               /* update past & sign */
            len = e_place - s_place + 1;    /* length of variable name */
            s_copy(&var,line_c,s_place,len);          /* variable name */
            f_sfield( var, f_prof,f_fmat );
            e_place = e_place + 2;                /* update past the ; */
            s_copy(&temp,line_c,e_place,0);    /* text to search still */
            }
         else  {                              /* really not a variable */
            s_copy(&temp,line_c,0,b_place); /* text upto space after & */
            f_render( f_fmat, temp, B_PRINTABLE, NIL ) ;
            b_place++;                    /* update past space after & */
            s_copy(&temp,line_c,b_place,0);    /* text to search still */
            }
         s_copy(&line_c,temp,0,0);        /* text to be searched still */
         }
      else  {                          /* no variables in line of text */
         f_render( f_fmat, line_c, B_PRINTABLE, NIL ) ;
         s_smp_ul(line) = 0;                   /* SB s_nulout(&line); */
         s_smp_ul(line_c) = 0;                 /* SB s_nulout(&line); */
         }
      }                                      /* no more text to search */

   s_drop( &line_c );
   s_drop( &blank );
   s_drop( &var   );
   s_drop( &temp  );
   return(S_NULL);
}
