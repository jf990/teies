/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_ntoe()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   target      string*  IN/OUT   string to be converted   |*/
/*|  :PARAM.   source      byte*    INPUT    conversion source        |*/
/*|  :PARAM.   beg         sint     INPUT    beginning of section     |*/
/*|  :PARAM.   len         sint     INPUT    length to do             |*/
/*|                                                                   |*/
/*|  :PURPOSE. Converts a section of a C-str to a TEIES string        |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns zero or standard STR error RC                  |*/
/*|                                                                   |*/
/*|  :CALLS.   s_alloc(), s_nulout(), s_ovrall(), s_calc              |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    19_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|                                    instead of calls to s_corrupt()|*/
/*|                                    ; removed checking of          |*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    17_Jan_88   K.J.Walsh   Added s_calc() call            |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"


extern s_alloc  () ;
extern s_nulout () ;
extern s_ovrall () ;

sint s_ntoe (target, source, beg, len)
string      *target ;             /* Conversion target -- TEIES string */
byte        *source ;             /* Conversion source -- C-string     */
sint         beg    ;             /* Beginning of indicated section    */
sint         len    ;             /* Length    of indicated section    */
{
   sint      clen   ;             /* Length of C-string                */
   sint      count  ;             /* Counter used for copying          */
   sint      rc     ;             /* Return code from s_alloc          */


   rc = s_nulout(target);

   for ( clen = 0 ; source[clen] != '\0' ; clen++ ) ;
   if ( clen > S_MAXLEN )
   {
   return(S_BAD_SIZE);
   }

   if (len < 0)                   /* Check for negative length         */
   {
   return(S_BAD_LEN);
   }
{
   if ( beg == 0 )
   {                                   /* If both are zero, use defalts*/
      if ( len == 0 || len > clen )       /* also OK if LEN is too long   */
      {
         len = clen;
      }
   }
   else if ( beg > 0 )                  /* When B is positive...        */
   {
      beg--;                            /* -- as 0 and 1 mean 1st byte  */
      if ( len == 0 )
      {                                /* LEN zero indicates entire str*/
         len = clen;
      }
      if ( beg > clen )                  /* BEG past last byte           */
      {
         len = 0;
      }
      else if ( beg + len > clen )        /* range bleeds right           */
      {
         len = clen - beg;                /* Cut length                   */
      }
   }
   else                                /* BEG is negative              */
   {
      beg = clen + beg;
      if ( beg >= 0 )
      {
         if ( len == 0 )
         {                             /* LEN zero indicates entire str*/
            len = clen;
         }
         if ( beg > clen )               /* BEG past last byte           */
         {
            len = 0;
         }
         else if ( beg + len > clen )     /* range bleeds right           */
         {
            len = clen - beg;             /* Cut length                   */
         }
      }
      else if ( len == 0 )              /* Default says use it all      */
      {
         beg = 0;
         len = clen;
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
   if (len == 0 )
   {
   return(0);
   }

   rc = s_alloc(target,len);

   for (count = 0 ; count <= len - 1 ; count++)   /* C O P Y        */
      s_char_s(*target, count) = source [beg+count] ;
   s_smp_ul(*target) = len ;   /* Set length in string structure    */

   return(0);
}
