/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_type()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 6, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   oid         OID *      INPUT  OID pointer to find      |
  |                                          object type of.          |
  |  <PURPOSE> Given an object identifier, return the type of object. |
  |            First the local SOIDI is checked.  If no entry is      |
  |            found locally the task is spawned off to the Master    |
  |            Server.                                                |
  |  <OUTPUT>  hword                        type of object            |
  |  <CALLS>   soidi_gt(), t_remote()                                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



hword   obj_type( oid_to_check )

OID              *oid_to_check;


{

   hword  type_of_object   =   0,
          lfd              =   0;
   tint   goid             =   0;
   sint   rc               =   0;




   if( OID_POINTER_OK( oid_to_check ) )
   {


      rc = soidi_gt( oid_to_check, & lfd, & goid, & type_of_object );

      if( rc != 0 )
      {


      }
   }




   return(type_of_object);

}
