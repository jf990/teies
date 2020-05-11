/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     JN_LIST C                                              |*/
/*| <LEVEL>    OBJ                                                    |*/
/*| <PARAM>    ocb        OCB*       IN   List to join                |*/
/*| <PARAM>    modifier   string     IN   Not used;                   |*/
/*| <PARAM>    level      hword      IN   Not used;                   |*/
/*| <PARAM>    params     byte*      IN   Not used;                   |*/
/*| <PURPOSE>  Gets list_fun() to invoke the Join action for each     |*/
/*|            item in the list.                                      |*/
/*| <OUTPUT>   sint                      Returns code from list_fun().|*/
/*| <CALLS>    list_fun()                                             |*/
/*| <MODS>     27_Mar_89   K.J.Walsh   Implemented.                   |*/
/*| </DOC>                                                            |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "obj.h"
#include "obj_prim.h"



sint jn_list( OCB *ocb, string modifier, hword level, byte *params )

{


   sint rcode = 0;


   rcode = list_fun(  JOIN_ACT, ocb, modifier, level, params  );

   return(rcode);

}
