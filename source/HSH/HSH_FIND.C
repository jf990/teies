/*+-------------------------------------------------------------------+
 | <DOC>                                                              |
 |                                                                    |
 | <NAME>       hsh_find.c                                            |
 |                                                                    |
 | <LEVEL>      HASH                                                  |
 |                                                                    |
 | <AUTHOR>     Samir Chopra                                          |
 |                                                                    |
 | <DATE>       May 31st, 1989                                        |
 |                                                                    |
 | <PARAM>      NAME          string       INPUT                      |
 |              HASH_PTR     pointer       INPUT                      |
 |                                                                    |
 | <GLOBALS>    None                                                  |
 |                                                                    |
 | <PURPOSE>    Routine searches for string variable in hash table    |
 |                                                                    |
 | <OUTPUT>     Pointer to location of string variable in hash table  |
 |                                                                    |
 | <ERRORS>     24003   HASH_BAD_POINTER_TO_CELL                      |
 |              NIL     indicates string not found                    |
 |                                                                    |
 | </DOC>                                                             |
 +--------------------------------------------------------------------+*/


#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "hash.h"
#include "debug.h"

 CELL   *hsh_find ( name , tab_ptr)

 string          name;
 CELL       **tab_ptr;

 {

    CELL     *np =  NIL,
             *rc =  NIL;
    sint       r =    0;




   if (tab_ptr == NIL)
   {
      rc = (CELL*)HASH_BAD_POINTER_TO_CELL;
   }
   else
   {
      np = *tab_ptr;
      for ( np = *(tab_ptr + hsh_calc(name)) ; np != NULL; np = np->next)
      {
         if (s_comp( name, np->v_name , 0, 0, 'F', &r ) == 0 )
            rc = np ;
      }
   }
   return(rc);
}
