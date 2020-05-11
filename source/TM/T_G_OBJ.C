/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_g_obj
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 7, 1988                                           |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server    string     INPUT    ID to whom to send object|
  |  <PARAM>   flag      hword      INPUT    Open flags               |
  |  <PARAM>   data      string     INPUT    OID to locate.           |
  |                                                                   |
  |                                                                   |
  |  <PURPOSE> Another server wants us to send out an object given    |
  |            that object's OID.                                     |
  |                                                                   |
  |  <OUTPUT>  sint                       error code.                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "debug.h"



sint t_g_obj( server, flag, data )

string  server;
hword   flag;
string  data;

{

 sint   flags     =      0,
        state     =      0,
        rc        =      0;
 OCB   *object_cb =    NIL;
 string result    = S_NULL;






   if( s_len( data ) > 1 )
   {

      object_cb = obj_opn( ((OID *) s_buffer( data )), OBJ_UPDATE );
      if( OCB_POINTER_OK( object_cb ) )
      {

         rc = t_m_obj(
                    server, object_cb, & result, FALSE, state
                     );
         if( rc != 0 )
         {

            except( rc, INFO, server, S_NULL, S_NULL, S_NULL, S_NULL );
            rc = 0;

         }
         rc = obj_cls( object_cb );

      }
      else
      {

         rc = OID_NOT_IN_SOIDI;

      }
   }
   else
   {

      rc = INVALID_OID;

   }

   if( rc != 0 )
   {

      rc = t_csend( server, rc, S_NULL );

   }

   return(rc);
}
