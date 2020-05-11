/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    D_strlen.c                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   source      byte*    INPUT    C-string to measure      |*/
/*|                                                                   |*/
/*|  :PURPOSE. Replaces the similarily named standard C function.     |*/
/*|            Some of the standard functions are required for testing|*/
/*|            but behave differently when certain control characters |*/
/*|            are encounted.                                         |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns tint string length                             |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    09_Dec_87   K.J.Walsh   Authored.                      |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
tint  D_strlen(str)
byte  str(::);
{
   tint  len   =  0  ;

   for ( len = 0 ; str(:len:) !=0 ; len++ ) ;

   len--;

   return(len);
}
