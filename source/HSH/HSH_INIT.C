/*+-------------------------------------------------------------------+
  | <DOC>                                                             |
  |                                                                   |
  | <NAME>     hsh_init.c                                             |
  |                                                                   |
  | <LEVEL>    HASH                                                   |
  |                                                                   |
  | <AUTHOR>   Samir Chopra                                           |
  |                                                                   |
  | <DATE>     June 1st, 1989                                         |
  |                                                                   |
  | <PARAM>    None                                                   |
  |                                                                   |
  | <GLOBALS>  None                                                   |
  |                                                                   |
  | <PURPOSE>  Routine initializes hash table by setting all pointers |
  |            to a NULL value.                                       |
  |                                                                   |
  | <ERRORS>                                                          |
  |                                                                   |
  | </DOC>                                                            |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "osk.h"
#include "str.h"
#include "obj.h"
#include "hash.h"
#include "debug.h"

 CELL **hsh_init( )


{

    CELL    **tab;


    tab = (CELL**)os_mema( sizeof (CELL*)* HASH_TABLE_SIZE);
    os_memn( tab,sizeof (CELL*)*HASH_TABLE_SIZE);
   return(tab);

}
