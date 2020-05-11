/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <NAME>    var_assign.c                                           |
  |                                                                   |
  |  <AUTHOR>  Samir Chopra                                           |
  |                                                                   |
  |  <DATE>    August 10, 1989                                        |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  TM_master->current->vars                               |
  |                                                                   |
  |  <PARAM>   name     string     INPUT   Name of variable to set.   |
  |                                                                   |
  |            contents pointer    INPUT   Data associated with name. |
  |                                                                   |
  |            type     hword      INPUT   Data type of variable      |
  |                                                                   |
  |  <PURPOSE> Set a variable name to a specific content.             |
  |                                                                   |
  |  <OUTPUT>  Always return 0.                                       |
  |                                                                   |
  |  <CALLS>   var_fnd(), var_add(), s_len(), s_upper()               |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
  |                                                                   |
  |  <LIMITS>  None known.                                            |
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


sint   var_assign( name, contents,type)

string              name;
void           *contents;
hword               type;

{

   CELL    *hash_ptr = NIL;


   s_upper( name, 0, 0 );


   if( s_len( name ) > 0 )

   {

      hash_ptr = var_fnd( name );
      if( hash_ptr == NIL )

      {

         hash_ptr= var_add( name,contents,type);

      }
      else
      hash_ptr = var_ext(name,contents,type,hash_ptr);


   }

   return(0);

}
