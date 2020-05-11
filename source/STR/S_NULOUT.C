/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    s_nullout()                                            |*/
/*|                                                                   |*/
/*|  :LEVEL.   str                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.   tsrt        string*  I/O      pointer to be zeroed.    |*/
/*|                                                                   |*/
/*|  :PURPOSE. Given a string pointer, it makes a null string.        |*/
/*|     This means it does nothing for an already null string;        |*/
/*|     or sets the usage length to zero for a simple string.         |*/
/*|     Does NOT check for corrupt strings.                           |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*|  :OUTPUT.  returns  sint zero                                     |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|  :MODS.    23_Mar_87   K.J.Walsh   Authored.                      |*/
/*|  :MODS.    23_May_87   K.J.Walsh   Brought return codes, documen- |*/
/*|                                    tation up to 1.42; put in calls|*/
/*|                                    s_corrup(); removed checking of|*/
/*|                                    allocation routines.           |*/
/*|  :MODS.    25_Nov_87   K.J.Walsh   Removed deallocation.          |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str_i.h"
#include "debug.h"


sint s_nulout (tstr)
string *tstr   ;                  /* String to be nulled               */
{

   if ( *tstr == S_NULL )         /* String is NULL                    */
   return(0);

   s_smp_ul(*tstr) = 0;
   return(0);
}
