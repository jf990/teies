/*+-------------------------------------------------------------------+
  | <DOC>                                                             |
  |                                                                   |
  | <NAME>     hsh_end.c                                              |
  |                                                                   |
  | <LEVEL>    HASH                                                   |
  |                                                                   |
  | <AUTHOR>   Samir Chopra                                           |
  |                                                                   |
  | <DATE>     July     , 1989                                        |
  |                                                                   |
  | <PARAM>    tab_ptr    POINTER     input                           |
  |                                                                   |
  | <GLOBALS>  None                                                   |
  |                                                                   |
  | <PURPOSE>  Routine shuts down HASH level, deallocating all space  |
  |            allocated for hash tables.                             |
  |                                                                   |
  | <OUTPUT>   Always return 0                                        |
  |                                                                   |
  | <ERRORS>   24003     HASH_BAD_POINTER_TO_CELL                     |
  |            24004     HASH_LEVEL_NOT_INITIALIZED                   |
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

 sint hsh_end ( root_ptr)

 CELL          **root_ptr

{
  sint   rc = 0;

  #D_begin('hsh_end','HASH');
  #D_show ('root_ptr =0x%x','root_ptr');
  #D_show ('*root_ptr =0x%x','*root_ptr');

  if (root_ptr == NIL)
  {
     #D_e_say('Error ! Pointer is not valid');
     rc = HASH_BAD_POINTER_TO_CELL;
   }
   else
   hsh_drop (root_ptr)
   #D_return(rc);
}
