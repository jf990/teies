/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_ctrim()                                              |*/
/*|  :AUTHOR.  John L. Foster                                         |*/
/*|  :DATE.    January 20, 1988                                       |*/
/*|                                                                   |*/
/*|  :LEVEL.   STR                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.  * none *                                               |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string*  IN/OUT   string to be trimmed     |*/
/*|  :PARAM.   where       byte     INPUT    L=lead, T=trail, B=both  |*/
/*|  :PARAM.   what        byte     INPUT    Char to trim             |*/
/*|                                                                   |*/
/*|  :PURPOSE. to remove leading and/or trailing occurances of a given|*/
/*|            character from the input string.                       |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.  Does not cut allocation if needed                      |*/
/*|                                                                   |*/
/*|  :MODS.    21_Jan_88   K.J.Walsh   Replaced lower case checking   |*/
/*|                                    with OS_TOUPPER(), added       |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"
#include "osk.h"




extern   sint  s_nulout () ;







sint s_ctrim ( tstr, where, what )

string        *tstr;
byte                 where, what ;




{


sint    rc     =  0     ,         /* RC from things for testing        */
        len    =  0     ,         /* Used length of string             */
        count  =  0     ,         /* Counter to be used in loops       */
        first  =  0     ,         /* Last non-blank character          */
        last   =  0     ,         /* First non-blank character         */
        end    =  0     ,         /* Points to last char in string     */
        new    =  0     ;         /* Points to where moving characters */
boolean blank  =  TRUE  ;         /* True until NON-blank reached      */








   if ( (len = s_len( *tstr ) ) == 0 )
   {
   return(0);
   }

   where = OS_TOUPPER(where);

   switch( where )
   {


   case 'B' :
   case 'L' :
   case 'T' :

/*+-------------------------------------------------------------------+*/
/*| If just removing TRAILING or BOTH occurances:                     |*/
/*+-------------------------------------------------------------------+*/
   if (where == 'T' || where == 'B' )
   {
      end = len - 1;                   /* Where the last char is       */
      last = end;                      /* Assume it is last non-blank  */
      while ( s_char_s (*tstr, last) == what )
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
/*| If just removing LEADING or BOTH occurances:                      |*/
/*+-------------------------------------------------------------------+*/
   if (where == 'L' || where == 'B')
   {
      first = 0 ;                      /* Assume 1st char of string    */
      end = len - 1;                   /* Where the last char is       */
      while ( s_char_s (*tstr, first) == what )
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
         s_smp_ul(*tstr) = new ;          /* New is now the length!    */
      }
   }
   break;

/*+-------------------------------------------------------------------+*/
/*| Here if invalid "where" arg                                       |*/
/*+-------------------------------------------------------------------+*/
   default :
           {
   return(S_BAD_PARM);
           }
   }


   return(0);
}
