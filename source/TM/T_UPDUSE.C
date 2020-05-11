/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_upduser()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    December 14, 1988                                      |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master                                              |
  |  <PARAM>   * none *                                               |
  |  <PURPOSE> Update the new item indexes of the users object, if    |
  |            they changed.                                          |
  |                                                                   |
  |            t_updcnt() is called on the MS to go through all nec-  |
  |            essary indexes and place the item counts into the user |
  |            object.  Then we ask the MS for the updated counters.  |
  |                                                                   |
  |            The data packet for this operation is the object ID    |
  |            concatenated with the component packet, as follows:    |
  |                                                                   |
  |           +-------+------------------+                            |
  |           |  OID  | COMPONENT_PACKET |                            |
  |           | 8bytes|    (ocd_pack)    |                            |
  |           +-------+------------------+                            |
  |                                                                   |
  |            Control is passed to t_rcvocd() when return packets    |
  |            come back from the master server.  T_rcvocd() unpacks  |
  |            the return data into the user object.  When t_waitc()  |
  |            returns either the update is complete or there was an  |
  |            error.                                                 |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   t_send(), t_author(), s_catb(), ocd_pack(), t_waitc()  |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint   t_upduser (   )


{

   sint        rc            =            0,
               state         =            0;
   hword       flags         =            0,
               command       = SEND_SEGMENT;
   string      packet        =       S_NULL,
               result_packet =       S_NULL;
   OCD        *component     =          NIL;



if(  MASTER_SERVER_IS_CONNECTED() )
{

   rc = t_remote( TM_master->MS, 0, 0, TM_UPDATE_USER_COUNTERS,
                                               t_author(), S_NULL, NIL );

   if( rc == 0 )
   {

      ocd_new( & component, "INDEXES" );

      s_catb( & result_packet, (byte *) & (TM_master->ucb->user_id),
                                                         sizeof( OID ) );
      rc = ocd_pack( & result_packet, component );

      rc = t_send( TM_master->MS, command, flags, state, result_packet );

      if( rc == 0 )
      {

         rc = t_waitc( TM_master->MS, & packet );


      }
      s_drop(  & result_packet );
      ocd_drp( & component     );

   }
}
else
{

   rc = MASTER_SERVER_IS_NOT_AVAILABLE;

}

   return(rc);

}
