/*+-------------------------------------------------------------------+
  | <DOC>                                                             |
  |                                                                   |
  | <NAME>     hsh_drop.c                                             |
  |                                                                   |
  | <LEVEL>    HASH                                                   |
  |                                                                   |
  | <AUTHOR>   Samir Chopra                                           |
  |                                                                   |
  | <DATE>     June 23rd, 1989                                        |
  |                                                                   |
  | <PARAM>    tab_ptr    POINTER     input                           |
  |                                                                   |
  | <GLOBALS>  None                                                   |
  |                                                                   |
  | <PURPOSE>  Routine deallocates space created for hash table       |
  |                                                                   |
  | <OUTPUT>   Always return 0                                        |
  |                                                                   |
  | <ERRORS>   24003     HASH_BAD_POINTER_TO_CELL                     |
  |                                                                   |
  | </DOC>                                                            |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "hash.h"
#include "debug.h"
#include "isv.h"

 sint hsh_drop( tab_ptr)

 CELL     **tab_ptr;

 {

   CELL   *hash_ptr     = NIL;
   CELL   *hash_new     = NIL;
   CELL   *hash_save    = NIL;
   sint   rc            =   0;
   sint   i             =   0;


   if (tab_ptr == NIL)

   {
      rc = HASH_BAD_POINTER_TO_CELL;
   }
   else
   {
      for ( i = 0; i!= HASH_TABLE_SIZE ; i++)
      {
         hash_new = *(tab_ptr + i);
         while (hash_new != NIL)
         {
            if ( hash_new->contents != NULL )
            {
               if (hash_new->type == OC_HASH)
               {
                  hsh_drop( hash_new->contents);
               }
                  os_memd( hash_new->contents );
                  hash_new->contents = NIL;
            }
            hash_save = hash_new->next;
            s_drop( &(hash_new->v_name) );
            os_memd (hash_new);
            hash_new = hash_save;
         }
      }
   }

   return(rc);

}
