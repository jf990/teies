/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_trim()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string*  IN/OUT   string to be trimmed     |*/
/*|  :PARAM.   where       byte     INPUT    L=lead, T=trail, B=both  |*/
/*|                                                                   |*/
/*|  :PURPOSE. to remove leading and/or trailing blanks from the      |*/
/*|            given string                                           |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.  Does not cut allocation if needed                      |*/
/*|                                                                   |*/
/*|  :MODS.    11_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    29_Oct_87   K.J.Walsh   Cut to no longer consider      |*/
/*|                                    complex strings; also improved |*/
/*|                                    efficiency & removed excessive |*/
/*|                                    PPPMAC debugging; put in test  |*/
/*|                                    bed                            |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "osk.h"
#include "debug.h"


extern   sint  s_nulout () ;

sint s_trim (tstr, where)
string  *tstr  ;                  /* TEIES string to be trimmed        */
byte    where ;                   /* Where to trim from                */
{
   sint rc     =  0     ;         /* RC from things for testing        */
   tint len    =  0     ;         /* Used length of string             */
   tint count  =  0     ;         /* Counter to be used in loops       */
   tint first  =  0     ;         /* Last non-blank character          */
   tint last   =  0     ;         /* First non-blank character         */
   tint end    =  0     ;         /* Points to last char in string     */
   tint new    =  0     ;         /* Points to where moving characters */
   bool blank  =  TRUE  ;         /* True until NON-blank reached      */


   len = s_len (*tstr) ;          /* Get used length of string         */
   if (len == 0)                  /* If string is empty                */
   {
   return(0);
   }
   where = OS_TOUPPER(where);
/*+-------------------------------------------------------------------+*/
/*| If just removing LEADING blanks:                                  |*/
/*+-------------------------------------------------------------------+*/
   if (where == 'L' )
   {
      first = 0 ;                      /* Assume 1st char of string    */
      end = len - 1;                   /* Where the last char is       */
      while ( s_char_s (*tstr, first) == ' ' )
      {
         if ( first == end )           /* If F=E string is all blanks! */
         {
            rc = s_nulout(tstr);       /* Null it out                  */
   return(0);
         }
         first++;                      /* Let's look at the next       */
      }

      /*+-------------------------------------------------------------+*/
      /*| If here then we found the 1st non-blank                     |*/
      /*+-------------------------------------------------------------+*/

      new = 0;                         /* copy tstr(first) to tstr(new)*/
      if ( first != 0 )                /* But only if needed!          */
      {
         while ( first <= end )
         {
               s_char_s (*tstr, new++) = s_char_s (*tstr, first++) ;
         }
         s_smp_ul(*tstr) = new ;          /* New is now the lenght!    */
      }
   }

/*+-------------------------------------------------------------------+*/
/*| If just removing TRAILING blanks:                                 |*/
/*+-------------------------------------------------------------------+*/
   else if (where == 'T' )
   {
      end = len - 1;                   /* Where the last char is       */
      last = end;                      /* Assume it is last non-blank  */
      while ( s_char_s (*tstr, last) == ' ' )
      {
         if ( last == 0 )              /* If L=0 string is all blanks! */
         {
            rc = s_nulout(tstr);       /* Null it out                  */
   return(0);
         }
         last--;                       /* Let's look at the next       */
      }

      /*+-------------------------------------------------------------+*/
      /*| If here then we found the last non-blank                    |*/
      /*+-------------------------------------------------------------+*/

      s_smp_ul(*tstr) = last + 1;      /* Just set usage length        */
   }

/*+-------------------------------------------------------------------+*/
/*| For both LEADING and TRAILING...                                  |*/
/*+-------------------------------------------------------------------+*/
   else if (where == 'B' )
   {
      first = 0 ;                      /* Assume 1st char of string    */
      end = len - 1;                   /* Where the last char is       */
      last = end;                      /* Assume it is last non-blank  */
      while ( s_char_s (*tstr, first) == ' ' )
      {
         if ( first == end )           /* If F=E string is all blanks! */
         {
            rc = s_nulout(tstr);       /* Null it out                  */
   return(0);
         }
         first++;                      /* Let's look at the next       */
      }

      /*+-------------------------------------------------------------+*/
      /*| When here we found the 1st non-blank:  find last non-blank  |*/
      /*+-------------------------------------------------------------+*/

      while ( s_char_s (*tstr, last) == ' ' )
      {
         last--;                       /* Let's look at the next       */
      }

      /*+-------------------------------------------------------------+*/
      /*| When here we found the last non-blank:  copy string         |*/
      /*+-------------------------------------------------------------+*/

      s_smp_ul(*tstr) = last-first+1;  /* Set new lenght               */

      new = 0;                         /* copy tstr(first) to tstr(new)*/
      if ( first != 0 )                /* But only if needed!          */
      {
         while ( first <= end )
         {
               s_char_s (*tstr, new++) = s_char_s (*tstr, first++) ;
         }
      }
   }

/*+-------------------------------------------------------------------+*/
/*| Here if invalid "where" arg                                       |*/
/*+-------------------------------------------------------------------+*/
   else
   {
   return(S_BAD_PARM);
   }

/*+-------------------------------------------------------------------+*/
/*| If execution get here, we think we have successfully done it.     |*/
/*+-------------------------------------------------------------------+*/
                                       /* Cut allocation done here     */
   return(0);
}
