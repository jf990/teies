/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             F_TAG_GET.C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Heidi E. Harting
:DATE.                             Sept 15, 1987

:INPUT.                line  :  a TEIES string from which the first
                                tagged qualifier will be parsed.

:OUTPUT.               qual  :  a TEIES string containing the value
                                of the first tagged qualifier parsed
                                from LINE.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -
:PURPOSE.
          This routine will parse the first text found with openand
          close delimiter tags, and return this value in the
          TEIES string QUAL.

:CALLS.   s_copy.c   s_first.c   s_init.c   s_trim.c

:ERRORS.

:LIMITS.  For now, all elements being searched for without dilimeters
          must be followed by one blank space.  The reason is that if
          no dilimeters are specified, a blank is assumed to follow the
          word.  This will have to be changed in the near future so this
          routine can be of more general use in the TEIES parsers.

:MOD.     Heidi:  This routine works as of September 16, 1987.

          Heidi - Nov 4, 1987 : routine modified to extract text between
                  either single quotes, double quotes, parenthesis, or
                  if no delimeters specified, the first word of text.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "debug.h"
#include "form_i.h"

string  f_tag_get(line)
string  line;

{
   sint  s_tag=0, e_tag=0;
   string  tag=S_NULL, qual=S_NULL, temp=S_NULL;
   string  s_quote=S_NULL, d_quote=S_NULL, o_parn=S_NULL, c_parn=S_NULL;

   #D_begin(f_tag_ge, PARSER)
   s_init(&s_quote,"'");                     /* delimeter single quote */
   s_init(&d_quote,"\"");
   s_init(&o_parn,"(");                  /* delimeter open parenthesis */
   s_init(&c_parn,")");                 /* delimeter close parenthesis */

   s_trim(&line,'L');                          /* strip leading blanks */
   s_tag = s_first(s_quote,line,0,0);        /* is single quote there? */
   if ( s_tag == 1 ) {                   /* must be first tagged value */
      s_copy( & tag, s_quote, 0, 0 );
      }
   else {
      s_tag = s_first(d_quote,line,0,0);     /* is double quote there? */
      if ( s_tag == 1 ) {
         s_copy( & tag, d_quote, 0, 0 );
         }
      else {
         s_tag = s_first(o_parn,line,0,0);  /* open parenthesis there? */
         if ( s_tag == 1 ) {
            s_copy( & tag, c_parn, 0, 0 );
            }
         else {
            s_tag = 0;                         /* will take first word */
            s_init(&tag," ");
            }
         }
      }
   s_tag++;                                   /* want 1 past start-tag */
   s_copy(&temp,line,s_tag,0);             /* copy text after open tag */
   e_tag = s_first(tag,temp,0,0);                /* search for end tag */
   if ( e_tag > 1 ) {                             /* the end tag found */
      e_tag--;                                /* want 1 before end-tag */
      }
   s_copy(&qual,line,s_tag,e_tag);          /* extract qualifier value */
   #D_str(qual,'The qualifier value: ')

   s_drop( & s_quote );
   s_drop( & d_quote );
   s_drop( & o_parn  );
   s_drop( & c_parn  );
   s_drop( & temp    );
   s_drop( & tag     );


   #D_return(qual)
}
