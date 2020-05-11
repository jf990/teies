/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     DO_LIST C                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    OBJ                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.    ocb        OCB*       IN   List to abstract;           |*/
/*| :PARAM.    modifier   string     IN   Not used;                   |*/
/*| :PARAM.    level      hword      IN   Not used;                   |*/
/*| :PARAM.    params     byte*      IN   Not used;                   |*/
/*|                                                                   |*/
/*| :PURPOSE.  Gets list_fun() to perform "do" on all the             |*/
/*|            items in the list.                                     |*/
/*|                                                                   |*/
/*| :OUTPUT.   Returns code from list_fun().                          |*/
/*|                                                                   |*/
/*| :CALLS.    list_fun()                                             |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     27_Mar_89   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "obj.h"
#include "obj_prim.h"
#include "debug.h"

sint do_list( OCB *ocb, string modifier, hword level, byte *params )

{
   sint rcode = 0;


   rcode = list_fun(DO_ACT, ocb, modifier, level, params);

   return(rcode);

}
