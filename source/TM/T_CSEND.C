/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_csend()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 27, 1988                                          |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   server      string     INPUT  Server to send a con-    |
  |                                            firmation to;          |
  |  <PARAM>   rc          sint       INPUT  Return code to report as |
  |                                            result of remote op.   |
  |  <PARAM>   data        string     INPUT  Supplimentary data to    |
  |                                            report back.           |
  |  <PURPOSE> A server has request a remote operation that is now    |
  |            complete.  Send return information back to that server.|
  |  <OUTPUT>  sint                                                   |
  |  <LIMITS>  Only sends confirmations to User Servers, not to system|
  |            servers such as the Master Server.  For now, there is  |
  |            no point in wasting CPU cycles processing confirmations|
  |            on the MS.  He will just assume the user server got the|
  |            message.                                               |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



sint   t_csend(  server, rc, data  )

string            server,    data;
sint              rc;



{

   string   packet  =  S_NULL;
   hword    flags   =       0;
   sint     state   =       0;




switch( TM_master->usrtype )
{

   case  MASTER_SERVER  :
   case  DBMS_SERVER    :
   case  NET_SERVER     :

            if( rc == 0 )
            {

               flags = TRANSACTION_SUCESSFUL;

            }
            else
            {

               flags = ERROR_IN_TRANSACTION;

            }
            s_init(  & packet, "____"                   );
            os_memc( & rc,     s_buffer( packet ), 4    );
            s_catx(  & packet, data,               0, 0 );
            rc = t_send( server, CONFIRMATION, flags, state, packet );
            s_drop(  & packet                           );
            break;


   case  REAL_USER      :

            break;

   default              :

            break;

}


   return(rc);

}
