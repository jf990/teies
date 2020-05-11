/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_inttoc()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   value       byte     INPUT    the value to be converted|*/
/*|                                                                   |*/
/*|  :PURPOSE. converts a numeric value to the appropriate CHARACTER  |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns a byte, the desired character                  |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    26_Aug_86   N.R.Voronka Written.                       |*/
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

byte s_inttoc (value)
byte value ;
{
   #D_start (s_inttoc, str, 'value=%d' , 'value')
#if (0x30 == '0')       /* Current character set is ASCII              */
   #D_return (0x30+value)
#else                   /* Current character set is EBCDIC             */
   #D_return (0xF0+value)
#endif
}
