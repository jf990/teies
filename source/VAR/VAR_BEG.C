/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |                                                                   |
  |  <NAME>    var_beg.c                                              |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    November 20, 1987                                      |

  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  TM_master->current->tm_pid                             |
  |                                                                   |
  |  <PARAM>   some_ptr  byte**  INPUT   Pointer to where we will save|
  |                                      the variables from.          |
  |                                                                   |
  |  <PURPOSE> Allocate the variable table header.                    |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>   hsh_init(), hsh_ins()                                  |
  |                                                                   |
  |  <ERRORS>  None                                                   |
  |                                                                   |
  |  <LIMITS>  None                                                   |
  |                                                                   |
  |  <MODS>    July_10_1989   Samir Chopra: Completely rewritten to   |
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
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "hash.h"


sint   var_beg( )



{

   string name = S_NULL;
   CELL         **tab = NIL;
   CELL         *hash_ptr = NIL;

   #D_begin(  var_beg,  VAR  );
   #D_show('Asked to initialize at loc=x%x.','TM_master->current->vars');

   s_init( &name,"$P_ID$");

   tab = hsh_init();

   #D_show('tab = 0x%x','tab');
   #D_show('*tab = 0x%x','*tab');


   TM_master->current->vars = tab;
   var_assign( name, &( TM_master->current->tm_pid ), OC_INT );

   #D_show ('TM_master-> = 0x%x, *TM_master->current->vars= 0x%x',\
   'TM_master->current->vars,*TM_master->current->vars');
   s_drop( &name );

   #D_return( 0 );
}
