/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    cfr_updv()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 7, 1990                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> Update the current users view of conference memberships|
  |            as they may have changed since signon or the last up-  |
  |            date.                                                  |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint cfr_updv( )


{

sint        rc            =     NO_ERROR;
hword       flags         =            0,
            command       = SEND_SEGMENT,
            state         =            0;
string      packet        =       S_NULL,
            result_packet =       S_NULL;
OCD       * component     =          NIL;



if(  MASTER_SERVER_IS_CONNECTED() )
{

   ocd_new(  & component,      "MEMBERSHIPS" );
   s_catb(   & result_packet,   (byte *) &(TM_master->ucb->user_id),
                                                           sizeof(OID) );
   ocd_pack( & result_packet,   component );

   rc = t_send( TM_master->MS,   command, flags, state, result_packet );
   if( rc == 0 )
   {

      rc = t_waitc( TM_master->MS, & packet );

   }
   ocd_drp( & component     );
   s_drop(  & result_packet );
   s_drop(  & packet        );

}

   return(rc);

}
