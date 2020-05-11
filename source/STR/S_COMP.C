/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_comp()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|   :PARAM.  str1     string  INPUT  string to be compared against  |*/
/*|   :PARAM.  str2     string  INPUT  string to be compared          |*/
/*|   :PARAM.  beg2     sint    INPUT  beg pos of section of str2     |*/
/*|   :PARAM.  len      sint    INPUT  length of section of str2      |*/
/*|   :PARAM.  cmp      byte    INPUT  char indicating type of cmp    |*/
/*|   :PARAM.  rtn      sint*  OUTPUT  indicates where cmp ended      |*/
/*|                                                                   |*/
/*|   :PURPOSE. Compares str1 with str2 within the range specied by   |*/
/*|             beg and len.  The method of comparison is cmp and     |*/
/*|             rtn indicates where the comparison terminated.        |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a standard STR error RC or                     |*/
/*|            ZERO to indicate that STR1 == STR2; or                 |*/
/*|            ONE  to indicate that STR1 >  STR2; or                 |*/
/*|            TWO  to indicate that STR2 >  STR1.                    |*/
/*|                                                                   |*/
/*|  :CALLS.   s_len(), s_plex(), #s_check,                           |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_PARM                                             |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|   :MODS.    26_May_87   N.Voronka   Authored.                     |*/
/*|   :MODS.    01_Jun_87   K.J.Walsh   Brought to 1.42 doc std, minor|*/
/*|                                     structure changes, error if   |*/
/*|                                     unsupported option asked for  |*/
/*|   :MODS.    01_Jun_88   K.J.Walsh   Added F ("Frills") option.    |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "str.h"
#include "osk.h"
#include "debug.h"


sint s_comp (str1, str2, beg2, complen, cmp, rtn)
string  str1      ;                    /* String against which to comp */
string  str2      ;                    /* String which is compared     */
sint    beg2      ;                    /* Beginning of str2 to compare */
sint    complen   ;                    /* Length    of str2 to compare */
byte    cmp       ;                    /* Method of comp:  A,E,L,I,M   */
sint   *rtn       ;                    /* RC inicating where cmp ended */
{
   sint len1      ;                    /* Used length of str1          */
   sint len2      ;                    /* Used length of str2          */
   byte chr1      ;                    /* Character from str1          */
   byte chr2      ;                    /* Character from str2          */
   sint beg1  = 0 ;                    /* Counter in comparison loop   */
   sint end2  = 0 ;                    /* Where to end comp (in STR 2) */
   bool difflen = TRUE;                /* Is STR1 length <> complen?   */
   bool bigcomp = FALSE ;              /* Is STR1 shorter than complen?*/




   if ( complen < 0 )
   {
   return(S_BAD_LEN);
   }

   len1 = s_len(str1);
   len2 = s_len(str2);

{
   if ( beg2 == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( complen == 0 || complen > len2 )       /* also OK if LEN is too long   */
      {
         complen = len2;
      }
   }
   else if ( beg2 > 0 )                  /* When B is positive...        */
   {
      beg2--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( complen == 0 )
      {                                /* LEN zero indicates entire str*/
         complen = len2;
      }
      if ( beg2 > len2 )                  /* BEG past last byte           */
      {
         complen = 0;
      }
      else if ( beg2 + complen > len2 )        /* range bleeds right           */
      {
         complen = len2 - beg2;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg2 = len2 + beg2;
      if ( beg2 >= 0 )
      {
         if ( complen == 0 )
         {                             /* LEN zero indicates entire str*/
            complen = len2;
         }
         if ( beg2 > len2 )               /* BEG past last byte           */
         {
            complen = 0;
         }
         else if ( beg2 + complen > len2 )     /* range bleeds right           */
         {
            complen = len2 - beg2;             /* Cut length                   */
         }
      }
      else if ( complen == 0 )              /* Default says use it all      */
      {
         beg2 = 0;
         complen = len2;
      }
      else if ( beg2 + complen < 0 )          /* Completely out of range      */
      {
         complen = 0;
      }
      else
      {                                /* Range bleeds left            */
         complen = complen + beg2;                 /* NB -- adding a negative      */
         beg2 = 0;
      }
   }
}



   cmp = OS_TOUPPER(cmp);              /* Upper case parameter         */

   switch (cmp)                        /* Go do comparision            */
   {
      /*+-------------------------------------------------------------+*/
      /*| M -> MACHINE COMPARE                                        |*/
      /*+-------------------------------------------------------------+*/
      case 'M':
      if( rtn != 0 && rtn != (sint *) -1 )
      {

         *rtn = 0;

      }
      len2 = complen;
      if ( len1 == 0 )
      {
         if ( len2 == 0 )
         {
   return(0);
         }
         else
         {
   return(2);
         }
      }
      if ( len2 == 0 )
      {
   return(1);
      }
      /*+-------------------------------------------------------------+*/
      /*| If STR1 is shorter than complen, the length of STR2 to be   |*/
      /*| checked, then we must not attempt to check the portion of   |*/
      /*| STR1 that does not exist.                                   |*/
      /*|                                                             |*/
      /*| To catch this, we set len2 to be complen, the lenght to     |*/
      /*| check, and then set complen to be len1 if shorter.          |*/
      /*+-------------------------------------------------------------+*/

      do
      {
         chr1 = s_char_s(str1,beg1++);
         chr2 = s_char_s(str2,beg2++);
         if ( chr1 > chr2 )
         {
            if( rtn != 0 && rtn != (sint *) -1 )
            {

               *rtn = beg1;

            }
   return(1);
         }
         else if ( chr1 < chr2 )
         {
            if( rtn != 0 && rtn != (sint *) -1 )
            {

               *rtn = - beg1;

            }
   return(2);
         }
      }
      while ( len1 > beg1 && len2 > beg2 );
      /*+-------------------------------------------------------------+*/
      /*| If we get here it is because either:                        |*/
      /*|   1: Both strings are equal in content and length;          |*/
      /*|   OR                                                        |*/
      /*|   2: Both strings are equal through the checked length but  |*/
      /*|      one is shorter than the other;                         |*/
      /*|                                                             |*/
      /*| Now we figgure out which of the two happened:               |*/
      /*+-------------------------------------------------------------+*/
      if ( len1 > beg1 )
      {
         if( rtn != 0 && rtn != (sint *) -1 )
         {

            *rtn = ++beg1;

         }
   return(1);
      }
      else if ( len2 > beg2 )
      {
         if( rtn != 0 && rtn != (sint *) -1 )
         {

            *rtn = - ++beg1;

         }
   return(2);
      }
      else
      {
         /*  (*rtn should already be 0 ) */
   return(0);
      }
      break;
      /*+-------------------------------------------------------------+*/
      /*| F -> "FANCY" OR "FRILLS" COMPARE                            |*/
      /*+-------------------------------------------------------------+*/
      case 'F':
      if( rtn != 0 && rtn != (sint *) -1 )
      {

         *rtn = 0;

      }
      len2 = complen;
      /*+-------------------------------------------------------------+*/
      /*| Start the ball rolling by cutting leading and trailing      |*/
      /*| blanks.  This is less efficient than cutting the trailing   |*/
      /*| blanks later, but less complicated to implement.            |*/
      /*+-------------------------------------------------------------+*/
      while( s_char_s(str1,beg1) == ' ' & len1 > beg1 )
      {                                /* Found a leading blank...     */
         beg1++;                       /* Increment the beg1          */
      }                                /*                              */
      while ( s_char_s(str2,beg2) == ' ' & len2 > beg2 )
      {                                /* Found a leading blank...     */
         beg2++;                       /* Move pointer up one          */
      }                                /*                              */
      len1--;
      len2--;
      while( s_char_s(str1,len1) == ' ' & len1 > 0 )
      {                                /* Found a trailing blank...    */
         len1--;                       /* Shorten the effective length */
      }                                /*                              */
      while ( s_char_s(str2,len2) == ' ' & len2 > 0 )
      {                                /* Found a trailing blank...    */
         len2--;                       /* Shorten the effective length */
      }                                /*                              */
      /*+-------------------------------------------------------------+*/
      /*| Now one or even both strings might be reduced to nothing    |*/
      /*| because it containted only blanks.                          |*/
      /*+-------------------------------------------------------------+*/
      if ( len1 < beg1 )
      {
         if ( len2 < beg2 )
         {
   return(0);
         }
         else
         {
   return(2);
         }
      }
      else if ( len2 < beg2 )
      {
   return(1);
      }
      /*+-------------------------------------------------------------+*/
      /*| OK, looks like we have two valid strings to check.  It is of|*/
      /*| particular importance to the following algorithm that the   |*/
      /*| last character of either string will *NOT* be a blank, as it|*/
      /*| skips all blanks until it finds a blank followed by a       |*/
      /*| nonblank.  If there were trailing blanks then it will try to|*/
      /*| check the character after the last blank (and that might be |*/
      /*| disaster!                                                   |*/
      /*+-------------------------------------------------------------+*/
      do
      {
         while( (chr1 = s_char_s(str1,beg1)) == ' ' \
            && len1 > beg1 \
            && s_char_s(str1,beg1+1) == ' ' )
         {                             /* Found 2 imbedded blanks...   */
            beg1++;                    /* Increment the pointer in S1 */
         }                             /*                              */
         while( (chr2 = s_char_s(str2,beg2)) == ' ' \
            && len2 > 0 \
            && s_char_s(str2,beg2+1) == ' ' )
         {                             /* Found 2 imbedded blanks...   */
            beg2++;                    /* Increment the pointer in S2 */
         }                             /*                              */
         chr1 = OS_TOUPPER(chr1);
         chr2 = OS_TOUPPER(chr2);
         beg2++;
         beg1++;
         if ( chr1 > chr2 )
         {
            if( rtn != 0 && rtn != (sint *) -1 )
            {

               *rtn = beg1;

            }
   return(1);
         }
         else if ( chr1 < chr2 )
         {
            if( rtn != 0 && rtn != (sint *) -1 )
            {

               *rtn = - beg1;

            }
   return(2);
         }
      }
      while ( len1 >= beg1 && len2 >= beg2 );
      if ( len1 >= beg1 )
      {
         if( rtn != 0 && rtn != (sint *) -1 )
         {

            *rtn = ++beg1 ;

         }
   return(1);
      }
      else if ( len2 >= beg2 )
      {
         if( rtn != 0 && rtn != (sint *) -1 )
         {

            *rtn = - ( ++beg1 );

         }
   return(2);
      }
      else
      {
         /*  (*rtn should already be 0 ) */
   return(0);
      }
      break;

      case 'A':                        /* ASCII comparison             */
      case 'E':                        /* EBCDIC comparison            */
      case 'L':                        /* Lexical comparison           */
      case 'I':                        /* Lexical/ignore case compare  */
   return(S_BAD_PARM);
      break;
      default:
   return(S_BAD_PARM);
   }
}                                      /*                              */
