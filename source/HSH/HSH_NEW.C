/*+-------------------------------------------------------------------+
  | <DOC>                                                             |
  |                                                                   |
  | <NAME>     hsh_new.c                                              |
  |                                                                   |
  | <LEVEL>    HASH                                                   |
  |                                                                   |
  | <AUTHOR>   Samir Chopra                                           |
  |                                                                   |
  | <DATE>     June 13th, 1989                                        |
  |                                                                   |
  | <PARAM>    None                                                   |
  |                                                                   |
  | <GLOBALS>  None                                                   |
  |                                                                   |
  | <PURPOSE>  Routine allocates and intializes a new hash table.     |
  |                                                                   |
  | <OUTPUT>   Pointer to new array of pointers to CELL               |
  |                                                                   |
  | <ERRORS>   None                                                   |
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

 CELL  **hsh_new()

 {
    CELL      **new_tab  = NIL;


    new_tab = (CELL **) os_mema(sizeof  (CELL *) * HASH_TABLE_SIZE  );

    os_memn( new_tab, sizeof (CELL *) * HASH_TABLE_SIZE );


   return(new_tab);

 }
