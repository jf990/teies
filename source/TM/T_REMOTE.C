/*+-------------------------------------------------------------------+
  |  <NAME>    t_remote()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 27, 1988                                          |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server      string     INPUT  Server to ask;           |
  |  <PARAM>   object      hword      INPUT  Object/Action request;   |
  |  <PARAM>   action      hword      INPUT                           |
  |  <PARAM>   level       hword      INPUT  State level modifier;    |
  |  <PARAM>   ocb         OCB *      INPUT  necessary ocb;           |
  |  <PARAM>   data        string     INPUT  additional data.         |
  |  <PARAM>   result      void *     OUTPUT Where to store results.  |
  |                                                                   |
  |  :PURPOSE. Send the server a request for a remote operation.  This|
  |            invokes a call to t_jmpst() on the requested server by |
  |            passing to it all the necessary routine parameters.  A |
  |            return code and pointer to return data can be used to  |
  |            learn of the results encountered by the request on that|
  |            server.  See t_jmpst() and t_do() for individual doc-  |
  |            umemtation on the function requestes possible.         |
  |            Packet format:                                         |
  |                                                                   |
  |              +--------+--------+-----+-------+--------+           |
  |              | Object | Action | OID | level | string |           |
  |              | 2 bytes| 2 bytes|  8  |2 bytes|        |           |
  |              |        |        |bytes|       |        |           |
  |              +--------+--------+-----+-------+--------+           |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



sint   t_remote(  server, object, action, level, ocb, data, result_str )

string            server,                             data;
void                                                       *result_str;
hword                      object, action, level;
OCB                                             *ocb;





{

   string   packet = S_NULL;
   sint     rc     =      0,
            state  =      0;
   hword    flags  =      0;





   s_init(  & packet, "OOAAooooooooLL5555" );
   os_memc( & object, s_buffer( packet ) + 0, 2 );
   os_memc( & action, s_buffer( packet ) + 2, 2 );
   if( ! OCB_POINTER_OK( ocb ) )
   {

      os_memn( s_buffer( packet ) + 4, sizeof( OID ) );

   }
   else
   {

      os_memc( & (ocb->oid), s_buffer( packet ) + 4, sizeof( OID ) );

   }
   os_memc( & level, s_buffer( packet ) + 12, 2 );
   if( s_len( data ) < 1 )
   {

      os_memn( s_buffer( packet ) + 14, 4 );

   }
   else
   {

      s_smp_ul( packet ) = 14;
      s_catx( & packet, data, 0, 0 );

   }


   t_send( server, OBJECT_ACTION, flags, state, packet );

   rc = t_waitc( server, result_str );

   s_drop( & packet );



   return(rc);

}
