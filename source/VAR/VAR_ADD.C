/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |                                                                   |
  |  <NAME>    var_add.c                                              |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    November 20, 1987                                      |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  TM_master->current->vars                               |
  |                                                                   |
  |  <PARAM>   name  string  INPUT  Name of new entry.                |
  |                                                                   |
  |  <PURPOSE> Create a new variable in the variables hash table      |
  |                                                                   |
  |  <OUTPUT>  Always return 0.                                       |
  |                                                                   |
  |  <CALLS>   s_parse(), hsh_ins(), os_memc()                        |
  |                                                                   |
  |  <ERRORS>  None known.                                            |
  |                                                                   |
  |  <LIMITS>  In the future, entries should be sorted.               |
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
#include "hash.h"
#include "vars.h"


CELL  *var_add( v_name,contents,d_type )

string          v_name;
void            *contents;
hword           d_type;

{

   CELL          *hash_ptr  =    NIL;
   string_vector  strvec1   = S_NULL;
   sint                 i   =      0,
                        r   =      0;





   if( TM_master->current->vars != NIL )
   {
#if 0
      hash_ptr = ( CELL *) os_mema (sizeof(CELL));
      os_memn(hash_ptr,sizeof(CELL));
#endif
      var_parse( v_name, & strvec1 );

      hash_ptr = hsh_ins ( *(strvec1+1), TM_master->current->vars);


      for ( i = 2; i<= s_vlen(strvec1); i++)

      {


         hash_ptr->type     = OC_HASH;
         hash_ptr->contents = hsh_init();
         hash_ptr           = hsh_ins( *(strvec1+i), hash_ptr->contents);


      }


      hash_ptr->type = d_type;

      switch (d_type)
      {
         case OC_STR:
         case OC_GFD:
         case OC_VAR:
         {
            hash_ptr->contents = S_NULL;
            s_copy( (string*)&(hash_ptr->contents), contents, 0, 0 );
            break;
         }
         case OC_BYTE:
         {
            os_memc(contents,hash_ptr->contents,1);
            break;
         }
         case  OC_INT:
         case  OC_REAL:
         {
            hash_ptr->contents = os_mema( 4 );
            os_memc(contents,hash_ptr->contents,4);
            break;
         }
         case  OC_D_T:
         {
            hash_ptr->contents = os_mema( sizeof( datetime ) );
            os_memc(contents,hash_ptr->contents,sizeof(datetime));
            break;
         }
         case  OC_PRIV:
         {
            hash_ptr->contents = os_mema( sizeof( PRIVS ) );
            os_memc(contents,hash_ptr->contents,sizeof(PRIVS));
            break;
         }
         case  OC_OID:
         case  OC_INHR:
         {
            hash_ptr->contents = os_mema( sizeof( OID ) );
            os_memc(contents,hash_ptr->contents,sizeof(OID));
            break;
         }
         case  OC_HWRD:
         case  OC_ISV:
         {
            hash_ptr->contents = os_mema( 2 );
            os_memc(contents,hash_ptr->contents,2);
            break;
         }
      }


      s_vdrop(&  strvec1);

   }
   else
   {


   }

   return(hash_ptr);

}
