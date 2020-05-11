/******************************************************************

   <DOC>

   <NAME>      ski_fmt.c

   <LEVEL>     SKI

   <AUTHOR>    Pamela Cham

   <DATE>      May 25, 1989

   <PARAMS>    result       string pointer   OUTPUT
               key_string   string           INPUT

   <PURPOSE>   This routine formats the given key_string into a
               valid keyword string result.  A slash is added at
               the start and at the end of key_string.  Each
               delimiting space in key_string is replaced by
               a slash to separate each word.  All excess spaces
               are deleted.

   <OUTPUT>    sint rc

   <CALLS>     s_parse(), s_valloc()

   <ERRS>      SKI_INVALID_CHARACTER      28330
               SKI_INVALID_STRING         28331

   </DOC>
******************************************************************/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"

sint ski_fmt (result, key_string)
string *result;
string key_string;
{
   sint errpos=0;
   sint rc=0;
   sint keystrlength=0;
   sint i=0;
   sint strvlen=0;                     /* used length of string vector */
   hword vectorsz=40;                  /* size for s_parse vector param*/
   string semicolon=S_NULL;
   string comma=S_NULL;
   string slash=S_NULL;
   string *strvector=NIL;              /* string vector  for s_parse   */
   string substr=S_NULL;               /* stores substrings extracted  */
   string extra=S_NULL;
   string parser=S_NULL;               /* template for s_parse         */

   #D_begin (ski_fmt, test);
   #D_str ('key_string', 'received key_string to parse');

   s_init (&semicolon, ";");
   s_init (&comma, ",");
   s_init (&slash, "/");

   if (&key_string != NIL  &&  key_string != S_NULL)
   {
      keystrlength = s_len (key_string);
                                       /* check for invalid chars      */
      errpos = s_first (semicolon, key_string, 1, keystrlength);
      if (errpos!=0)
      {
         #D_say ('invalid string: semicolon not allowed');
         rc = SKI_INVALID_CHARACTER;
      }
      else
      {
         errpos = s_first (comma, key_string, 1, keystrlength);
         if (errpos!=0)
         {
            #D_say ('invalid string: comma not allowed');
            rc = SKI_INVALID_CHARACTER;
         }
         else
                                       /* string is valid              */
         {                             /* trim and convert to upper    */
                                       /*   case before processing     */
            #D_say ('string is ok to parse');
            s_trim (&key_string, 'B');
            #D_str ('key_string', 'trimmed keystring');
            s_upper (key_string, 1, keystrlength);

                                       /* template to look for blanks  */
                                       /*   as delimiters              */
            s_init (&parser, "@~ .. @n .");
            s_nulout (result);
            s_valloc (&strvector, vectorsz);
            s_parse (&key_string, parser, S_NULL, strvector, &extra);
            #D_strvec ('strvector', 'string to get');

            strvlen = s_len ((string)strvector);
            for (i=1;  i <= strvlen;  i++)
            {                          /* separate substrings with a   */
                                       /*   slash                      */
               s_copy (&substr, *(strvector+i), 0, 0);
               #D_str ('substr', 'substrings extracted');
               s_cat (result, *result, slash, 0, 0);
               s_cat (result, *result, substr, 0, 0);
               #D_str ('*result', 'result after concatenation');
            }
            s_cat (result, *result, slash, 0, 0);
            #D_str ('*result', 'result after concatenation');
         }
      }
      s_drop (&substr);
      s_drop (&extra);
      s_drop (&parser);
   }
   else
   {
      #D_say ('invalid string');
      rc = SKI_INVALID_STRING;
   }
   #D_return (rc);
}



