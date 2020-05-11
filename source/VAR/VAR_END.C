/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <NAME>    var_end.c                                              |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    November 20, 1987                                      |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  TM_master->current->vars                               |
  |                                                                   |
  |  <PARAM>   * none *                                               |
  |                                                                   |
  |  <PURPOSE> Shut down VARS level, deallocating all space allocated |
  |            for variables and tables created for them.             |
  |  <OUTPUT>  Always return 0.                                       |
  |                                                                   |
  |  <CALLS>   os_memd(), s_drop()                                    |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
  |                                                                   |
  |  <LIMITS>  None known.                                            |
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

sint   var_end( root_ptr  )

CELL   **root_ptr;
{


if( (root_ptr!= NULL ))

{

   hsh_drop( root_ptr);
   os_memd(root_ptr);
   root_ptr = NIL;
}

else
{


   if(TM_master->current->vars != NIL )
   {

      hsh_drop(  (CELL **) TM_master->current->vars );
      os_memd( TM_master->current->vars );
      TM_master->current->vars = NIL ;

   }
}

   return(0);

}
