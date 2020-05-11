/*+-------------------------------------------------------------------+
 | <DOC>                                                              |
 |                                                                    |
 | <NAME>     hsh_copy.c                                              |
 |                                                                    |
 | <LEVEL>    HASH                                                    |
 |                                                                    |
 | <AUTHOR>   Samir Chopra                                            |
 |                                                                    |
 | <DATE>                                                             |
 |                                                                    |
 | <PARAM>    HASH_PTR      pointer       INPUT                       |
 |                                                                    |
 | <GLOBALS>  None                                                    |
 |                                                                    |
 | <PURPOSE>  Routine makes a copy of an input CELL structure.        |
 |                                                                    |
 | <OUTPUT>   Pointer to copy of CELL structure                       |
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

CELL   *hsh_copy( hash_ptr)

CELL   *hash_ptr;

{

   CELL     *hash_new = NIL;

   hash_new = (CELL*)os_mema(sizeof(CELL));
   os_memc ( hash_ptr, hash_new, sizeof(CELL));

   return(hash_new);

}
