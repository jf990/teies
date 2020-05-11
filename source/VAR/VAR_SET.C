/*+-------------------------------------------------------------------+
  |                                                                   |
  |  <NAME>    var_set.c                                              |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    November 20, 1987                                      |
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
  |  <PURPOSE> Set a variable name to a specifi content.              |
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
  |            Aug_23_88   JF    Altered initial checking to make sure|
  |                              both Name and Contents are specified.|
  |                                                                   |
  |            Aug_20_89   S.Chopra  Routine now calls var_assign to  |
  |                                  set contents. Done to preserve   |
  |                                  original parameters.             |
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


sint   var_set( name, contents)

string              name;
void           *contents;

{




   if( s_len( name ) > 0 )

   {

      var_assign(name,contents,OC_STR);
   }

   return(0);

}
