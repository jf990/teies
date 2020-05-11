/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    os_parse()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   OSK                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   output      string*  IN/OUT   target string            |*/
/*|  :PARAM.   input       string   INPUT    input string to parse    |*/
/*|  :PARAM.   ptr         sint*    IN/OUT   pointer into target      |*/
/*|  :PARAM.   maxlen      sint     INPUT    max allowed len of output|*/
/*|  :PARAM.   glen        sint     INPUT    actual total lenght of   |*/
/*|                                          the source               |*/
/*|                                                                   |*/
/*|  :PURPOSE. to separate portions of filedescriptors; parses a      |*/
/*|            given string starting at the given pointer and then    |*/
/*|            skips white space, finds a string, stopping when (1)   |*/
/*|            trailing white space is encountered or (2) the found   |*/
/*|            string is greater in length than the call-line specified*/
/*|            maxlen.  An indicated output pointer is then set to    |*/
/*|            indicate that string, and the input pointer is set to  |*/
/*|            the character after the last character extracted.      |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or a negative value to indicate error     |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.  -1, -2                                                 |*/
/*|                                                                   |*/
/*|  :LIMITS.  Error codes -1 & -2 should be changed to follow the    |*/
/*|            grouping currently in use in the OS level              |*/
/*|                                                                   |*/
/*|            If the element to be parsed is greater than the Max    |*/
/*|            allowed, then only the Max allowed number of chars are |*/
/*|            returned BUT the next element position pointer is left |*/
/*|            at the end of the element.  Therefore in this case     |*/
/*|            *ptr <> usedlen and on return usedlen will be set to   |*/
/*|            maxlen.                                                |*/
/*|                                                                   |*/
/*|            A current big limit that has to be solved is the fact  |*/
/*|            that a non-valid file character causes this routine    |*/
/*|            to exit on an error condition.  What should happen is  |*/
/*|            that bad character converted to a valid file character |*/
/*|            (for example, underscore "_") and parsing continues.   |*/
/*|            It is not an error to have a bad character in the file |*/
/*|            name since in some OS's it may in fact be valid.       |*/
/*|                                                                   |*/
/*|  :MODS.    27_Apr_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    03_Dec_87   K.J.Walsh   Changed from os_okfd[] to    |*/
/*|                                    os_char[] table.             |*/
/*|  :MODS.    08_Jun_88   K.J.Walsh   If nothing extracted, "*" is   |*/
/*|                                    returned.                      |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"

sint os_parse(output,input,ptr,maxlen,glen)

string   *output  ;                    /* What we give 'em             */
string   input    ;                    /* What they gave us            */
sint     *ptr     ;                    /* Pointer into the input       */
sint     maxlen   ;                    /* It may not be bigger         */
sint     glen     ;                    /* Actual lenght of input str   */

{

   sint  usedlen  =  0  ;              /* Space used by this portion   */
   while ( (*ptr) < glen && s_char_s(input,(*ptr)) == ' ')
   {
      (*ptr)++;                        /* Skip over leading blanks     */
   }


   usedlen = 0;
   while ( (*ptr) < glen && s_char_s(input,(*ptr)) != ' ')
   {
      if ( os_char[(sint) s_char_s(input,(*ptr))] && OS_FD_CHAR )
      {
         s_char_s(*output,usedlen) = s_char_s(input,(*ptr));
         (*ptr)++;
         if ( usedlen++ > maxlen )
         {
            usedlen = maxlen;
         }
      }
      else                                            /* yes-OK for FD */
      {                                               /* No--get mad   */
         s_char_s(*output,usedlen) = '_';             /* See Limits    */
         usedlen++;
      }
   }

   if ( usedlen == 0 )
   {
      s_char_s(*output,0) = '*';       /* If we found nothing, then    */
      s_smp_ul(*output) = 1;           /* set it to "*", wildcharacter */
   }
   else
   {
      s_smp_ul(*output) = usedlen;
   }

   return(0);
}
