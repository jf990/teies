/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    var_fnd.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 20, 1987                                      |
  |                                                                   |
  |  :LEVEL.   VAR                                                    |
  |                                                                   |
  |  :GLOBAL.  TM_master->current->CELL                               |
  |                                                                   |
  |  :PARAM.   name   string    INPUT   Name of variable entry to find|
  |                                                                   |
  |  :PURPOSE. Locate the entry corresponding to the variable name    |
  |            requested.                                             |
  |  :OUTPUT.  CELL *                                                 |
  |                                                                   |
  |  :CALLS.   s_comp(),hsh_find(), s_parse().                        |
  |                                                                   |
  |  :ERRORS.  NIL : indicates stem not found in hash table.          |
  |                                                                   |
  |  :LIMITS.  None known                                             |
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

CELL  *var_fnd(  v_name  )

string           v_name;

{

   CELL    *hash_ptr      =   NIL,
           *loc_ptr       =   NIL;
   sint     i             =     0;
   string_vector  strvec  = NIL;







   if( TM_master->current->vars != NIL )
   {

      var_parse (  v_name,  & strvec  );

      hash_ptr = hsh_find ( *(strvec+1), TM_master->current->vars);
      loc_ptr = hash_ptr;

      while (hash_ptr!= NIL && hash_ptr->type == OC_HASH)

      {

         for ( i = 2 ; i <= s_vlen(strvec); i++)
         {
         loc_ptr = hash_ptr;

         hash_ptr = hsh_find (* (strvec + i),(CELL**)hash_ptr->contents);
         }
     }
     s_vdrop( & strvec );

  }
  else
  {


  }

   return(loc_ptr);

}
