/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     GT_LIST C                                              |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <PARAM>    ocb        OCB*       IN   List to abstract;           |*/
/*| <PARAM>    modifier   string     IN   Not used;                   |*/
/*| <PARAM>    level      hword      IN   Not used;                   |*/
/*| <PARAM>    params     byte*      IN   Not used;                   |*/
/*| <PURPOSE>  Gets list_fun() to perform a "get" of all the          |*/
/*|            items in the list.                                     |*/
/*| <OUTPUT>   Returns code from list_fun().                          |*/
/*| <CALLS>    list_fun()                                             |*/
/*| <MODS>     27_Mar_89   K.J.Walsh   Implemented.                   |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "obj.h"
#include "obj_prim.h"
#include "debug.h"

sint gt_list( OCB *ocb, string modifier, hword level, void *params )

{
   sint rcode = 0;


   rcode = list_fun(GET_ACT, ocb, modifier, level, params);

   return(rcode);

}
