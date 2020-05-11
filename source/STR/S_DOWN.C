/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_down()                                               |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   char        byte     INPUT    Input character to lower |*/
/*|                                                                   |*/
/*|  :PURPOSE. converts a single character to lower case              |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns passed character, except when character is     |*/
/*|            upper case, in which case the lower case translation   |*/
/*|            is passed                                              |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    27_Aug_86   N.R.Voronka Written.                       |*/
/*|  :MODS.    25_Jun_87   K.J.Walsh   Brought return codes,          |*/
/*|                                    documentation up to 1.42;      |*/
/*|  :MODS.    25_Jan_88   K.J.Walsh   No longer needed.              |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

byte s_down (chr)
byte chr ;
{
   #D_start (s_down, str, 'chr=%c', 'chr')
#if (0x30 == '0')                 /* If character is ASCII             */
      if (chr >= 'A' && chr <= 'Z')    /* If character is uppercase    */
         #D_return (chr + 0x20)        /*    convert and return        */
      else                             /* Else                         */
         #D_return (chr)               /*    just return               */
#else                             /* Else character is EBCDIC          */
      if (chr >= 'A' && chr <= 'I' || chr >= 'J' && chr <= 'R' ||
         chr >= 'S' && chr <= 'Z')     /* If character is uppercase    */
         #D_return (chr - 0x40)        /*    convert and return        */
      else                             /* Else                         */
         #D_return (chr)               /*    just return               */
#endif
}
