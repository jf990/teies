/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_HASHIT C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.   D_htrans   *sint      IN   Hashing translate table     |*/
/*|                                                                   |*/
/*| :PARAM.    inptr      *byte      IN   Pointer to character string |*/
/*|                                       to be hashed.               |*/
/*| :PARAM.    id         *byte      IN   String identifying caller   |*/
/*|                                                                   |*/
/*| :PURPOSE.  Calculates the hash value of the input string.  Hashes |*/
/*|            until a non-hashable character is found (particularily |*/
/*|            blank or null).  Any character not in the set of       |*/
/*|            {0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZ} is not hashable|*/
/*|                                                                   |*/
/*| :OUTPUT.   Hash value (even if zero, meaning unhashable).         |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     09_Jun_88   K.D.Ng      Implemented.                   |*/
/*| :MODS.     15_Jun_88   K.J.Walsh   Documented, minor alterations. |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "debug_i.h"

hword D_hashit( inptr , id )
byte *inptr;
byte *id;
{

   hword value    =  0  ;              /* Value to be returned         */
   byte  *ourptr  =  0  ;              /* Our copy of inptr            */
   hword transval =  0  ;              /* Conversion of a char in table*/

   ourptr = inptr;                     /* Copy pointer                 */
   while ( (transval = (hword) D_htrans(: *ourptr :)) != 0 )
   {
      value = value * 2 + transval ;
      if (value > 256)                 /* If value has wrapped, cut it */
      value = (value % 256) + ( value / 256);
      ourptr++;                        /* Look at next character       */
   }
   #if (NOT PROD_SYS)
      if ( D_SWITCH(D2HASHIT) || D_SWITCH(D5DEBUG) )
      {
         D_pr("!!!!:%s:D_hashit(A):  R:%d for \"%s\".\n",id,value,inptr);
      }
   #endif
   return(value);
}

