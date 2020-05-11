/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  :NAME.    isv_lkup()                                             |*/
/*|                                                                   |*/
/*|  :LEVEL.   isv                                                    |*/
/*|                                                                   |*/
/*|  :GLOBAL.                                                         |*/
/*|                                                                   |*/
/*|  :PARAM.                                                          |*/
/*|                                                                   |*/
/*|  :PURPOSE.                                                        |*/
/*|                Returns the index ( a number ) of the given        |*/
/*|                tag ( a string ).  This is the SLOW way of         |*/
/*|                finding an ISV entry.  The index is useful         |*/
/*|                only to the isv_get() function.                    |*/
/*|                                                                   |*/
/*|  :OUTPUT.                                                         |*/
/*|                                                                   |*/
/*|  :CALLS.                                                          |*/
/*|                                                                   |*/
/*|  :ERRORS.                                                         |*/
/*|                                                                   |*/
/*|  :LIMITS.  This dummy version is provided to facilitate headerfile|*/
/*|            and txtlib generation.                                 |*/
/*|                                                                   |*/
/*|  :MODS.    31_Mar_87   K.J.Walsh   Written.                       |*/
/*|  :MODS.    21_Jun_87   K.J.Walsh   Documented to 1.42 standards   |*/
/*|                                                                   |*/
/*|  :END.                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "str_i.h"
#include "isv_i.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "debug.h"

sint isv_lkup(tag)

string   tag   ;                 /* Identifies ISV entry to be found   */

{

   #D_start('isv_lkup','isv','tag=0x%X','tag');
   #D_str(tag,'to be found');

   #D_leave(0,'DUMMY VERSION');

}
