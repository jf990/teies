/*+-------------------------------------------------------------------+*/
/*| <DOC>                                                             |*/
/*| <NAME>     HSH_DUMP.C                                             |*/
/*|                                                                   |*/
/*| <LEVEL>    HASH                                                   |*/
/*|                                                                   |*/
/*| <GLOBAL>   start      pointer    IN   Pointer to start of CELL    |*/
/*|                                                                   |*/
/*| <PARAM>    caller     char       IN   ID of caller                |*/
/*|                                                                   |*/
/*| <PURPOSE>  To display contents of CELL structures in a hash table.|*/
/*|                                                                   |*/
/*| <OUTPUT>   None.                                                  |*/
/*|                                                                   |*/
/*| <CALLS>    Hsh_dump()    In case of recursion.                    |*/
/*|                                                                   |*/
/*| <ERRORS>   None.                                                  |*/
/*|                                                                   |*/
/*| <LIMITS>   None known                                             |*/
/*|                                                                   |*/
/*| <MODS>     07_Aug_89   Samir Chopra    Implemented.               |*/
/*|                                                                   |*/
/*| </DOC>                                                            |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "hash.h"
#include "debug.h"

void hsh_dump(start,caller)

CELL   **start;
char   *caller;

{
   CELL *pointer = NIL;
   sint i = 0;
   for ( i = 1 ; i!= HASH_TABLE_SIZE; i++)
   {
       pointer = *(start + i);
       while (pointer !=NIL)
      {
         switch ( pointer->type )
         {
            case OC_HASH:
               hsh_dump(pointer->contents,"RECURSIVE");
               break;
            case OC_STR:
               break;
            default:
               break;
         }
         pointer = pointer->next;
         break;
      }
   }
}
