/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    var_lfnd.c                                             |
  |                                                                   |
  |  <AUTHOR>  Samir Chopra                                           |
  |                                                                   |
  |  <DATE>    August 7, 1989                                         |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  TM_master->current->vars                               |
  |                                                                   |
  |  <PARAM>   name   string    INPUT   Name of variable entry to find|
  |                                                                   |
  |  <PURPOSE> To return a pointer to the last segment of a variable  |
  |            of the form Name.Name.                                 |
  |                                                                   |
  |  <OUTPUT>  CELL *                                                 |
  |                                                                   |
  |  <CALLS>   s_comp().                                              |
  |                                                                   |
  |  <ERRORS>  NIL : indicates stem doesn't exist.                    |
  |                                                                   |
  |  <LIMITS>  None known                                             |
  |                                                                   |
  |  <MODS>                                                           |
  |                                                                   |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "hash.h"

CELL  *var_lfnd(  name  )

string           name;

{

   CELL    *hash_ptr      =   NIL;
   sint     i             =     0;
   string_vector  strvec  = NIL;






   var_parse ( name, & strvec );

   hash_ptr = hsh_find ( *(strvec+1), TM_master->current->vars);

   while (hash_ptr!= NIL && hash_ptr->type == OC_HASH)

   {

      for ( i = 2 ; i <= s_vlen(strvec); i++)

      hash_ptr = hsh_find (* (strvec + i),(CELL**)hash_ptr->contents);
   }
   s_vdrop(& strvec    );

   return(hash_ptr);

}
