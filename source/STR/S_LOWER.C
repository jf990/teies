/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_lower                                                |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tstr        string*  IN/OUT   target string            |*/
/*|  :PARAM.   beg         sint     INPUT    from where to start      |*/
/*|  :PARAM.   len         sint     INPUT    how much to do           |*/
/*|                                                                   |*/
/*|  :PURPOSE. converts the indicated section of the input string to  |*/
/*|            lower case                                             |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :ERRORS.  S_BAD_LEN                                              |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    27_Aug_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "osk_cset.h"
#include "debug.h"


extern s_down    () ;

sint s_lower (tstr, beg, len)
string  tstr  ;              /* String to convert to uppercase         */
sint    beg   ;              /* Begining of section to convert         */
sint    len   ;              /* Length of section to convert           */
{
   sint lens  ;              /* Used length of 'tstr'                  */
   sint count ;              /* Counter in conversion loop             */



   if (len < 0)                   /* Check for negative length         */
   {
   return(S_BAD_LEN);
   }
   lens = s_len (tstr) ;          /* Get used length of source         */

{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > lens )       /* also OK if LEN is too long   */
      {
         len = lens;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = lens;
      }
      if ( beg > lens )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > lens )        /* range bleeds right           */
      {
         len = lens - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = lens + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = lens;
         }
         if ( beg > lens )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > lens )     /* range bleeds right           */
         {
            len = lens - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = lens;
      }
      else if ( beg + len < 0 )          /* Completely out of range      */
      {
         len = 0;
      }
      else
      {                                /* Range bleeds left            */
         len = len + beg;                 /* NB -- adding a negative      */
         beg = 0;
      }
   }
}

   if ( len == 0 )
   {
   return(0);
   }

   for (count = beg; count < beg+len;count++) /* C O N V E R T */
   {
      if ( OS_ISUPPER( s_char_s(tstr,count) ) )
      {
         s_char_s(tstr,count) = s_char_s(tstr,count) + OS_LO_DIFF ;

      }
   }

   return(0);
}
