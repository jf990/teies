/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                             FQUAL C
:LEVEL.                            FORMS.PARSER
:AUTHOR.                           Stephen Muccione
:DATE.                             June 8, 1989

:INPUT.                qual  :  a TEIES string containing the
                                potential qualifier.
                       m_quals: a f_qual_s qualifier array

:OUTPUT.               rc    :  the id of the qualifier.

:INPUT_GLOBALS.        - none -
:OUTPUT_GLOBALS.       - none -

:PURPOSE.
          This routine takes a string an checks it against a forms
          qualifier array and returns the id if a match is found or
          zero if no match.

:CALLS.   s_copy.c   s_stosi.c   s_type.c

:ERRORS.

:LIMITS.

:MODS.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "str.h"
#include "debug.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "form_i.h"

sint    f_qual ( qual, m_quals )

string          qual;
f_qualifiers  m_quals;

{
   string   tmp_str = S_NULL;
   sint     rc      = 0;
   sint     r       = 0;
   sint     i1      = 0;
   sint     id      = 0;


   s_copy (&tmp_str, qual, 0, 0);
   s_upper (tmp_str, 0, 0);

   id = 0;
   for (i1 = 1; i1 <= s_vlen(m_quals.qual); i1++)
   {
      if (s_comp(tmp_str,*(m_quals.qual + i1),0,0,'M',&r) == 0)
      {
         id = m_quals.id [i1];
         break;
      }
   }
   s_drop( & tmp_str );


   return (id);
}
