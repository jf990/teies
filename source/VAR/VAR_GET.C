/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <NAME>    var_get.c                                              |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    November 20, 1987                                      |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  * none *                                               |
  |                                                                   |
  |  <PARAM>   name   string   INPUT   Name to get.                   |
  |                                                                   |
  |  <PURPOSE> Return a pointer to the contents of the variable       |
  |            requested.                                             |
  |                                                                   |
  |  <OUTPUT>  void pointer to contents                               |
  |                                                                   |
  |  <CALLS>   var_fnd(),s_upper()                                    |
  |                                                                   |
  |  <ERRORS>  NIL:  Entry not found.                                 |
  |                                                                   |
  |  <LIMITS>  None known.                                            |
  |                                                                   |
  |  <MODS>    June_10_1989   Samir Chopra: Completely rewritten to   |
  |                                         accomodate variables of   |
  |                                         the form Name.Name.Name   |
  |                                         Routine uses hash tables  |
  |                                         as a storage mechanism.   |
  |                                         For definition of CELL    |
  |                                         structure, refer to hash.h|
  |                                         Old version is discarded  |
  |                                         and no longer used.       |
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

void  *var_get(  name  )

string           name;

{

   CELL    *hash_ptr         = NIL;
   void    *return_contents = NIL;


if( TM_master->current->vars != NIL )
{

   s_upper( name, 0, 0 );
   hash_ptr = var_lfnd( name );
   if( hash_ptr != NIL )

   {

      return_contents = hash_ptr->contents;

    }

}
else
{


}


   return(return_contents);

}
