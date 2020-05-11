/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     DEBUG H                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Required for all files using standardized debugging.   |*/
/*|            Includes header files needed by debugging and          |*/
/*|            the actual external debugging header file.             |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     23_Mar_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized / Trimmed MODS.   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef DEBUG_H_INCLUDED
#  define DEBUG_H_INCLUDED

#  ifndef DEFS_H_INCLUDED
#     error "DEFS.H" required by "DEBUG.H"

#  elif ( NOT PROD_SYS )               /* Include only if PRODUCTION   */
#     include "debug_x.h"              /* compile.  OSK_CSET.H is here */
#     include "osk_cset.h"             /* so there is no OSK dependancy*/
#  endif

#endif
