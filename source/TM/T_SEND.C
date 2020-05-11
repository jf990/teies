/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_send.c                                               |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 3, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   server      string    INPUT   Who to send this to      |
  |  <PARAM>   command     hword     INPUT   TM command requested     |
  |  <PARAM>   flags       hword     INPUT   Request flags - TM.H     |
  |  <PARAM>   state       hword     INPUT   obj LEVEL indicator      |
  |  <PARAM>   data        string    INPUT   Data packet to send to   |
  |                                          other server.            |
  |                                                                   |
  |  <PURPOSE> Send a request for the service of another TEIES server.|
  |            Typically this is peer to peer communications, where   |
  |            this TM will only request services of the other TM.    |
  |            TM.H #defines the flag masks and the TM requests.      |
  |                                                                   |
  |            If no connection exists with the destination server,   |
  |            attempt to establish one.  If that fails, then report  |
  |            an error tot eh exception handler and back to the      |
  |            caller (non-fatal error).                              |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   isf_send(), os_memc(), os_memd(), os_mema(), s_sml_ul()|
  |            s_smp_al(), sizeof(),  s_len()                         |
  |  <ERRORS>                                                         |
  |            28075 : Server out of service.                         |
  |            28076 : Invalid request, service not done.             |
  |            28077 : Server busy.                                   |
  |  <LIMITS>  None known.                                            |
  |  <MODS>    May 16, 1989      JF       Added STATE parameter.      |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "isf.h"
#include "tm.h"



sint   t_send( server, command, flags, state, data )

string         server;
hword                  command;
hword                           flags;
sint                                   state;
string                                        data;


{

string        request       = S_NULL;
sint          buffer_length = 0;
sint          rc            = 0;





buffer_length =  s_len( data ) + TM_PACKET_HEADER_SIZE;

s_alloc( & request, buffer_length );           /************************/
                                               /* Set the string used  */
                                               /* and allocated sizes  */
                                               /*                      */
os_memc( & command, s_buffer(request),  2  );  /* Now move in the Cmd  */
                                               /*                      */
                                               /* Now the flags        */
os_memc( & flags,   s_buffer(request) + TM_PACKET_FLAGS_OFFSET,  2  );

                                               /* Now the state ID     */
os_memc( & state,   s_buffer(request) + TM_PACKET_STATE_OFFSET,  4  );

                                               /************************/
if( s_len( data ) > 0 )                        /*                      */
{                                              /*                      */
                                               /* Finally the actual   */
                                               /* string of data to    */
                                               /* send.                */
                                               /************************/

   os_memc(s_buffer( data ), s_buffer( request ) + TM_PACKET_DATA_OFFSET,
                                            buffer_length - S_OVERHEAD );

}
s_smp_ul( request ) = buffer_length;


rc = isf_send( server, rc, request );
if( ISCF_NOT_CONNECTED( rc ) )
{

   rc = t_opncon( server, 0 );
   if( rc == 0 )
   {

      rc = isf_send( server, rc, request );

   }
}


s_drop( & request );


   return(rc);

}
