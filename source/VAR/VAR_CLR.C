/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <NAME>    var_clr.c                                              |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    November 20, 1987                                      |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  * none *                                               |
  |                                                                   |
  |  <PARAM>   name   string   INPUT  Variable entry to clear out.    |
  |                                                                   |
  |  <PURPOSE> Clear out the contents of a variable.                  |
  |                                                                   |
  |  <OUTPUT>  Always return 0.                                       |
  |                                                                   |
  |  <CALLS>   var_fnd(), hsh_drop(), os_memd()                       |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
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
  | </DOC>                                                            |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "hash.h"

sint   var_clr( name )

string          name;

{

   CELL   *hash_ptr = NIL;


   hash_ptr = var_lfnd( name );

   if( hash_ptr != NIL)
   {
      if (hash_ptr->type == OC_HASH)
      {
         hsh_drop(hash_ptr->contents);
      } else
      {
         os_memd (hash_ptr->contents);
      }
      hash_ptr->contents = NIL;
   }
   return(0);
}
