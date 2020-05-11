/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |                                                                   |
  |  <NAME>    var_ext.c                                              |
  |                                                                   |
  |  <AUTHOR>  Samir Chopra                                           |
  |                                                                   |
  |  <DATE>    August 7, 1989                                         |
  |                                                                   |
  |  <LEVEL>   VAR                                                    |
  |                                                                   |
  |  <GLOBAL>  TM_master->current->vars                               |
  |                                                                   |
  |  <PARAM>   name      string  INPUT  Name of new entry.            |
  |            contents  void*   INPUT  Pointer to data of variable   |
  |            type      hword   INPUT  Data type of variable         |
  |            hash_ptr  CELL *  INPUT  Pointer to CELL structure in  |
  |                                     hash table.                   |
  |                                                                   |
  |  <PURPOSE> Add the extensions of a compound variable to a hash    |
  |            table                                                  |
  |                                                                   |
  |  <OUTPUT>  Always return 0.                                       |
  |                                                                   |
  |  <CALLS>   s_parse(), hsh_ins(),os_memc                           |
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
#include "hash.h"
#include "vars.h"


CELL  *var_ext( name,contents,type,hash_ptr)

string          name;
void            *contents;
hword          type;
CELL           *hash_ptr;
{

   string_vector strvec1   =  S_NULL;

   sint                i   =      0 ,
                       r   =      0;




   var_parse( name, & strvec1 );

   for (i=1;(i<=s_vlen(strvec1))&& \
   (s_comp(*(strvec1+i),hash_ptr->v_name,0,0,'F',&r)==0);i++);

   if (i<= s_vlen(strvec1) )

   {

      while(i<=s_vlen(strvec1) )

      {

         hash_ptr = hsh_ins( *(strvec1+i), hash_ptr->contents);
         i++;
         if(i<=s_vlen(strvec1) )

         {

            hash_ptr->contents = hsh_init();
            hash_ptr->type     = OC_HASH;

         }

       }

       hash_ptr->type = type;

     }


   switch (type)
   {
      case OC_STR:
      case OC_GFD:
      case OC_VAR:
      {
         hash_ptr->contents = S_NULL;
         s_copy( (string*)&hash_ptr->contents,contents,0,0);
         break;
      }
      case OC_BYTE:
      {
         hash_ptr->contents = os_mema( 1 );
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
         hash_ptr->contents = os_mema( sizeof( datetime) );
         os_memc(contents,hash_ptr->contents,sizeof(datetime));
         break;
      }
      case  OC_PRIV:
      {
         hash_ptr->contents = os_mema( sizeof( PRIVS) );
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

   s_vdrop ( & strvec1 );



   return(hash_ptr );
}
