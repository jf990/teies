/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |                                                                   |
  |  <NAME>    obj_recover                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 27, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   object_id   OID *      INPUT   OID to recover.         |
  |                                                                   |
  |  <PURPOSE> Given an Object ID, restore the object in the local    |
  |            data base, providing for links to other objects and    |
  |            removing the entry from the deletion queue.            |
  |  <OUTPUT>  OCB *                                                  |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




OCB   * obj_recover(  object_id  )

OID                 * object_id;



{

   sint        rc         =      0;
   OCB       * return_ocb =    NIL;




if( OID_POINTER_OK( object_id ) )
{


}
else
{

   rc = INVALID_OID_POINTER;

}

   return(return_ocb);

}
