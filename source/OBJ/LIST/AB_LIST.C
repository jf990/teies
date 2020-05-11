/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     AB_LIST C                                              |*/
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
/*| :PURPOSE.  Gets list_fun() to display the abstracts of the        |*/
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

sint ab_list( OCB *ocb, string modifier, hword level, byte *params )

{
   sint rcode = 0;

   #D_begin( ab_list, OBJ );

   rcode = list_fun(ABSTRACT_ACT, ocb, modifier, level, params);

   #D_leave( rcode, 'Returned by list_fun()');

}
