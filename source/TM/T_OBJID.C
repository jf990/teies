/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_objid                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 25, 1989                                       |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server      string     INPUT                           |
  |  <PARAM>   object_id   OID *      INPUT                           |
  |  <PARAM>   result      string *   INOUT                           |
  |                                                                   |
  |  <PURPOSE> A server needs to know the Item Id of an object given  |
  |            the object's OID.                                      |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   t_objid(  server, object_id, result  )

string           server;
OID            * object_id;
string         * result;


{

   sint        rc         =      0;
   string      item_id    = S_NULL;




   rc = obj_id(  object_id, & item_id );
   rc = t_csend( server, rc,  item_id );

   if( result != NIL )
   {

      s_copy( result, item_id, 0, 0 );

   }
   s_drop( & item_id );


   return(rc);

}
