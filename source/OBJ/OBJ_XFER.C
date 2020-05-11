/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_xfer()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    May 1, 1989                                            |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  Object to get/put data.  |
  |  <PARAM>   component   OCD *      INPUT  Component of object.     |
  |  <PARAM>   server      string     INPUT  Who to send/receive.     |
  |  <PARAM>   direction   hword      INPUT  Direction of data travel.|
  |                                                                   |
  |  <PURPOSE> Send or request either a portion of an object or a     |
  |            whole object from another server.  If component is NIL |
  |            or the component name is length 0 then the whole object|
  |            is the target of the operation.                        |
  |                                                                   |
  |            Direction determines if this routine sends data in a   |
  |            local object to the target server or requests the      |
  |            server to send specified data in the object.  Values   |
  |            for direction are:                                     |
  |                                                                   |
  |               OBJ_XFER_REQUEST_COMPONENT                          |
  |               OBJ_XFER_REQUEST_OBJECT                             |
  |               OBJ_XFER_SEND_COMPONENT                             |
  |               OBJ_XFER_SEND_OBJECT                                |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
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




sint   obj_xfer( object, component, server, direction );

OCB            * object;
OCD            * component;
string           server;
hword            direction;
{

   sint        rc         =      0;


#D_begin(  obj_xfer,  OBJ  );

if ( OCB_POINTER_OK( object ) )
{

   if ( component == NIL || s_len( component->name ) < 1 )
   { /* work with object */
   }
   else
   { /* work with component */
   }
}
else
{

   rc = INVALID_OCB;

}

                  OBJ_XFER_REQUEST_COMPONENT
                  OBJ_XFER_REQUEST_OBJECT
                  OBJ_XFER_SEND_COMPONENT
                  OBJ_XFER_SEND_OBJECT

#D_return( 'rc' );

}
