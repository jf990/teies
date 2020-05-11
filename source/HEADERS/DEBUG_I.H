/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    debug_i.h                                              |*/
/*|                                                                   |*/
/*|  :LEVEL.   debug                                                  |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE. Includes certain files needed internally by debugging  |*/
/*|            routines.  Note that debug_x.h is included even if     |*/
/*|            PROD_SYS is true.                                      |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.                                                         |*/
/*|                                                                   |*/
/*|                                                                   |*/
/*| :MODS.     29_Jul_88   K.J.Walsh   debug_i.h split into debug_i.h |*/
/*|                                    and debug_ii.h                 |*/
/*|                                                                   |*/
/*| :MODS.     15_Nov_88   K.J.Walsh   Added <stdio.h> and "defs.h"   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef DEBUG_I_H_INCLUDED

#   define DEBUG_I_H_INCLUDED

#   include <file.h>
#   include <stdio.h>
#   include "defs.h"
#   include "osk_cset.h"
#   include "debug_x.h"
#   include "debug_ii.h"

#endif
