/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    var_drp.c                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 20, 1987                                      |
  |  <LEVEL>   VAR                                                    |
  |  <GLOBAL>  TM_master->current->vars                               |
  |  <PARAM>   name       string     INPUT                            |
  |  <PURPOSE> Terminate all usage of input interface variable and    |
  |            release associated space.                              |
  |  <OUTPUT>  Always return 0.                                       |
  |  <CALLS>   os_memd(), var_fnd(), hsh_drop(), hsh_del()            |
  |  <MODS>    Aug_7_1989     Samir Chopra: Completely rewritten to   |
  |                                         accomodate variables of   |
  |                                         the form Name.Name.Name   |
  |                                         Routine uses hash tables  |
  |                                         as a storage mechanism.   |
  |                                         For definition of CELL    |
  |                                         structure, refer to hash.h|
  |                                         Old version is discarded  |
  |                                         and no longer used.       |
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


sint   var_drp(  name  )

string           name;

{

   CELL         * hash_ptr1 = NIL;
   CELL         * hash_ptr2 = NIL;
   string_vector  strvec    = NIL;




   if( TM_master->current->vars != NIL )

   {
      hash_ptr1 = var_fnd (name);
      if (hash_ptr1 !=NIL)
      {

         var_parse( name, & strvec );


         hash_ptr2 = hsh_find (*(strvec + s_vlen(strvec)),\
                                                    hash_ptr1->contents);
         if (hash_ptr2->type == OC_HASH)
         {
            hsh_drop(hash_ptr2->contents);
            os_memd(hash_ptr2->contents);
         }
         else
         {
            os_memd( hash_ptr2->contents );
         }
         hash_ptr2->contents = NIL;
         hsh_del(*(strvec +s_vlen(strvec)), hash_ptr1->contents);

         s_vdrop(& strvec );

      }
      os_memd (hash_ptr2);
   }

   return(0);

}
