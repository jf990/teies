/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_key                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 6, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   component    OCD *     INPUT   OCD to add key to.      |
  |  <PARAM>   key_source   void *    INPUT   Key to place in OCD.    |
  |  <PARAM>   key_length   sint      INPUT   Size of key.            |
  |                                                                   |
  |  <PURPOSE> Convert generic pointer data into a key to be used in  |
  |            the OCD provided.                                      |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   os_memc(), s_alloc(), s_smp_ul()                       |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   obj_key(  component,  source_key, key_length  )

OCD  *           component;
void *           source_key;
sint             key_length;

{

   sint        rc         =      0;






if( OCD_POINTER_OK( component ) )
{

   if( DATA_POINTER_OK( source_key ) )
   {

      s_alloc(  & (component->key),    key_length  );
      s_smp_ul(    component->key  ) = key_length   ;
      os_memc( source_key, s_buffer( component->key ), key_length );


   }
   else
   {

      rc = INVALID_DATA_POINTER;

   }
}
else
{

   rc = OCD_POINTER_INVALID;

}




   return(rc);

}
