/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_up()                                                 |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   char        byte     INPUT    character to be uppered  |*/
/*|                                                                   |*/
/*|  :PURPOSE. converts a chararter to upper case                     |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns the same byte or the upper case of the given   |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    27_Aug_87   N.R.Voronka Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    to #s_check() instead of  calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    25_Jan_88   K.J.Walsh   No longer needed.              |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str_i.h"
#include "debug.h"

byte s_up (chr)
byte chr ;
{
   #D_start (s_up, str, 'char=%c', 'chr')
#if (0x30 == '0')                 /* If character is ASCII             */
      if (chr >= 'a' && chr <= 'z')    /* If character is lowercase    */
         #D_return (chr - 0x20)        /*    convert and return        */
      else                             /* Else                         */
         #D_return (chr)               /*    just return               */
#else                             /* Else character is EBCDIC          */
      if (chr >= 'a' && chr <= 'i' || chr >= 'j' && chr <= 'r' ||
          chr >= 's' && chr <= 'z')    /* If character is lowercase    */
         #D_return (chr + 0x40)        /*    convert and return        */
      else                             /* Else                         */
         #D_return (chr)               /*    just return               */
#endif
}
