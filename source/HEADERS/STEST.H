/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     STEST H                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    STEST                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Includes actual STEST header files, which had to be    |*/
/*|            split because the MAKE file resulting from MAKER was   |*/
/*|            too big for the braindead Waterloo MAKE facility to    |*/
/*|            process.                                               |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     29_Jun_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized.                  |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef STEST_H_INCLUDED
#   define STEST_H_INCLUDED
#   include "stest_i1.h"
#   include "stest_i2.h"
#   include "stest_i3.h"
#   if ( PROD_SYS )
#      include "debug_x.h"
#   endif
#endif
