/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_r_obj()
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 7, 1988                                           |
  |  <LEVEL>   TM                                                     |
  |  <Param>   server    string     INPUT    to whom to send request  |
  |            mode      hword      INPUT    object open mode         |
  |            oid       OID  *     INPUT    OID we think we want here|
  |                                                                   |
  |  <PURPOSE> To receive an object from another server we give the   |
  |            server name and the OID for him to give us.  Then do   |
  |            a t_waitc() to invoke t_s_obj() to get the packed      |
  |            object into our  database.                             |
  |                                                                   |
  |            If the request goes as planned, we will return the OCB |
  |            of the object as it exists locally.  If we could not   |
  |            get the object NIL will be returned.  There can be 2   |
  |            reasons for this:  either the OID did not exist on the |
  |            requested server OR we asked for it in a mode that the |
  |            server could not comply with.                          |
  |                                                                   |
  |  <OUTPUT>  OCB *                                                  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "debug.h"



OCB *t_r_obj( server, mode, oid )

string        server;
hword         mode  ;
OID          *oid   ;

{

 OCB    *ocb  = NIL;
 sint   flags = 0,
        state = 0,
        rc    = 0;
 string data  = S_NULL;





   if( OID_POINTER_OK( oid ) &&  ( ! OID_IS_NULL( oid ) ) )
   {

      s_init( & data, "====----" );
      os_memc(  oid, s_buffer(data), sizeof( OID ) );
      rc = t_send( server, GET_OBJECT, mode, state, data );
      if( rc == 0 )
      {

         rc = t_waitc( server, NIL );

      }
      ocb = obj_opn( oid, mode );
      s_drop( & data );

   }
   else
   {

      rc = INVALID_OID;

   }

   if( rc != 0 )
   {

      except( rc, CONTROL_ERROR, server, S_NULL, S_NULL, S_NULL, S_NULL);

   }

   return(ocb);
}
