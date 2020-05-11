/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_owner                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    February 12, 1990                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object      OCB *      INPUT  Object to find owner of. |
  |  <PARAM>   owner_oid   OID **     INOUT  Pointer to OID pointer to|
  |                                           store result.           |
  |  <PURPOSE> Return the owner OID of a given object, if one exists. |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




sint   obj_owner(   OCB * object, OID ** owner_oid  )
{

   sint        rc         =      0;


#D_start( 'OBJ_OWNER', 'OBJ', 'Object=x%x; OID *=x%x.', 'object,owner_oid' );



if( OCB_POINTER_OK( object ) )
{

   if( owner_oid != NIL && owner_oid != (OID **) -1 )
   {

      switch( object->type )
      {
      }

   }
   else
   {

      rc = CANNOT_STORE_RESULT_OID;

   }
}
else
{

   rc = INVALID_OCB;

}



#D_return( 'rc' );

}
