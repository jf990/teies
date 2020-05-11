/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_s_obj
  |  <AUTHOR>  Tanmay S Kumar                                         |
  |  <DATE>    June 7, 1988                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   server    string     INPUT    ID to whom to send data  |
  |  <PARAM>   object  OCB *        INPUT    Pointer to the OCB that  |
  |                                          contains the pointer to  |
  |                                          the ODT                  |
  |  <PARAM>   flag      hword      INPUT                             |
  |  <PARAM>   data      string     INPUT                             |
  |                                                                   |
  |  <PURPOSE> To receive an object from a specified user id and to   |
  |            send a confirmation when the data is error free. The   |
  |            object is send in obj_pack() format so use obj_unpack()|
  |            to take it apart.  Ocb are passed through to unpack.   |
  |            Flags points to a hword to store a result code if      |
  |            needed.  Once the object is unpacked call the respec-  |
  |            tive lk_ routine and then send a confirmation to the   |
  |            requesting server.                                     |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"



sint t_s_obj(  server, object, flags, state, data  )

string         server;
OCB         ** object;
hword          flags;
string         data;

{

 sint      rc         =      0;
 hword     level      =      0;
 string    modifier   = S_NULL,
           result_str = S_NULL;
 OCB      *result_ocb =    NIL;







   if ( s_len(data) >= 4 )
   {
                                        /*+++++++++++++++++++++++++++++*/
                                        /*  If the pointer passed can- */
                                        /*  not be used to store the   */
                                        /*  resulting OCD then fake it */
                                        /*  with a local pointer.      */
                                        /*+++++++++++++++++++++++++++++*/
      if( object == NIL )
      {

         object = & result_ocb;

      }

      rc = obj_unpack( object, data );

      if(
           ( flags              &  OK_TO_LINK_OBJECT )
        && ( rc                 == 0                 )
        && ( TM_master->usrtype != USER_SERVER       )         )
      {

         level = (hword) state;
         rc    = t_jmpst( (*object)->type, LINK_ACT, level,
                                         modifier, * object, & result_str );

      }

      if( rc != DO_NOT_CONFIRM_TRANSACTION )
      {

         t_csend( server, rc, result_str );

      }
      else
      {

         rc = 0;

      }
      s_drop( & result_str );

      if( OCB_POINTER_OK( result_ocb ) )
      {

         obj_cls( result_ocb );

      }
   }


   return(rc);
}
