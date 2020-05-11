/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_recv.c                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 3, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   server      string    INPUT   Server to receive from.  |
  |  <PARAM>   command     hword *   INOUT   TM command requested to  |
  |                                          be filled in after recv. |
  |  <PARAM>   flags       hword *   INOUT   Request flags to be      |
  |                                          filled in after receive  |
  |  <PARAM>   data        string *  INOUT   Pointer to the data to be|
  |                                          filled in and returned.  |
  |                                                                   |
  |  <PURPOSE> Receive a command, data packet and flags from another  |
  |            TEIES server.  You need to know the server to receive  |
  |            from.  If there is something waiting from that server, |
  |            the data, flags and command will be filled in.         |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   isf_recv(), os_memc(), os_mema(), s_init()             |
  |  <ERRORS>                                                         |
  |            xx075 : Server out of service.                         |
  |            xx076 : Invalid request, service not done.             |
  |            xx077 : Server busy.                                   |
  |            xx078 : Nothing to receive from Server requested.      |
  |  <LIMITS>                                                         |
  |  <MODS>    May 16, 1989       JF      Added STATE parameter to    |
  |                                        parameters and packet.     |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "isf.h"
#include "tm.h"



sint   t_recv( server, command, flags, state, data )

string         server;
hword                * command;
hword                         * flags;
sint                                 * state;
string                                      * data;


{

string        request       = S_NULL,
              receive_buffer= S_NULL;
sint          rc            =      0,
              level         =      0;






   rc = isf_recv( server, level, & receive_buffer  );
   if( rc != 0 )
   {

      rc = TM_ISF_LEVEL_ERROR + ( rc % 100 );

   }
   else
   {

      if( command == NIL )
      {


      }
      else
      {

         os_memc( s_buffer( receive_buffer ), command, 2 );

      }
      if( flags == NIL )
      {


      }
      else
      {

         os_memc( s_buffer( receive_buffer ) + TM_PACKET_FLAGS_OFFSET,
                                                              flags, 2 );

      }
      if( state == NIL )
      {


      }
      else
      {

         os_memc( s_buffer( receive_buffer ) + TM_PACKET_STATE_OFFSET,
                                                              state, 4 );

      }
      if( s_len( receive_buffer ) > TM_PACKET_HEADER_SIZE )
      {

         s_copy(  data, receive_buffer, TM_PACKET_DATA_OFFSET + 1, 0 );

      }
      s_drop( & receive_buffer );

   }

   return(rc);

}
