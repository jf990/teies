/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     D_PR_NIL C                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    DEBUG                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    (ignored)                                              |*/
/*|                                                                   |*/
/*| :PURPOSE.  If D_pr() is set to "inhibit debug output", it is      |*/
/*|            set to point to this routine so that no printing is    |*/
/*|            done.                                                  |*/
/*|                                                                   |*/
/*| :OUTPUT.   output                                                 |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     29_Jun_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include <stdlib.h>
#include "debug_i.h"

sint D_pr_nil()
{
   return(0);
}
