/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_stopms                                               |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 26, 1988                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  TM_master                                              |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Shut down the master server from a remote ID.          |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isf.h"

#define ALREADY_CONNECTED 55364




sint   t_stopms()


{

hword    command = MS_SHUT_DOWN,
         flags   = 0;
string   server  = S_NULL,
         request = S_NULL;
sint     rc      = 0,
         state   = 0,
         spare   = 0;





   s_init( & request, "SIGNOFF MASTER_SERVER"   );
   rc = isf_open( TM_master->MS, spare, & spare );
   if( rc == 0 || rc == ALREADY_CONNECTED      )
   {

      os_wait( 1 );
      t_send(   TM_master->MS, command, flags, state, request );
      rc = 0;
      t_waitc(  TM_master->MS, NIL );
      isf_clos( TM_master->MS, spare );

   }

   return(rc);

}
