/*+-------------------------------------------------------------------+
 | <DOC>                                                              |
 |                                                                    |
 | <NAME>     hsh_ins.c                                               |
 |                                                                    |
 | <LEVEL>    HASH                                                    |
 |                                                                    |
 | <AUTHOR>   Samir Chopra                                            |
 |                                                                    |
 | <DATE>     May 31st, 1989                                          |
 |                                                                    |
 | <PARAM>    NAME          string        INPUT                       |
 |            HASH_PTR      pointer       INPUT                       |
 |                                                                    |
 | <GLOBALS>  None                                                    |
 |                                                                    |
 | <PURPOSE>  Routine inserts string variable  in hash table          |
 |                                                                    |
 | <OUTPUT>   Pointer to string variable in hash table                |
 |                                                                    |
 | <ERRORS>   None                                                    |
 |                                                                    |
 | </DOC>                                                             |
 +--------------------------------------------------------------------+*/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "hash.h"
#include "debug.h"
#include "isv.h"

 CELL   *hsh_ins (name , tab)

 string     name;
 CELL       **tab;

 {

    CELL           *np     =    NIL;
    sint       hashval     =      0;
    string      d_name     = S_NULL;


    s_copy ( &d_name, name, 0 ,0 );


 if (( np = (CELL *) hsh_find (d_name , tab)) == NULL)

    {

       np = (CELL *) os_mema (sizeof ( CELL));

       os_memn( np, sizeof (CELL));

       s_copy( &(np->v_name),d_name ,  0, 0 );

       np->next = NULL;

       hashval = hsh_calc (np->v_name);
       np->next = *(tab+ hashval);
       *(tab+hashval) = np;


    }
   s_drop (&d_name);
   return(np);
}
