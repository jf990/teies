/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 SCV_SEND.C

:LEVEL.                OSK - SCV

:AUTHOR.               Sal Johar

:DATE.                 April 16, 1987

:INPUT.            vmid   :  TEIES string with server id to send to
                   buffer :  pointer to transmit data

:OUTPUT.           rc     :  0 if send, else error condition.

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
            function: to send msg to any valid vm.

:CALLS.

:ERRORS.    SCV_ERR + SUB_ERR +
            00  :  General error
            01  :  Invalid Path-id
            03  :  Message limit exceeded.
            65  :  scv_on() was never called
            66  :  No path exists to the requested server.

:LIMITS.

:MODS.

            02/14/89      s_just() ----> s_pad()

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

#define SUB_ERR 500

sint   scv_send( vmid, buf )

string           vmid;            /* server to send buffer to          */
string                 buf;       /* pointer to the buffer             */
{
   sint    rc     = 0,
           pathid = 0;
   struct scv_conq *conprev = NULL;       /* ptr of the entry in list */
   string tmpvmid = S_NULL;


 if ( SCVExtrn != NULL)
 {
   s_pad( &tmpvmid, vmid, IDLEN, ' ', 'R');

   if (( rc = scv_qpth( tmpvmid, &conprev )) == ERR_ALREADY_CONN )
   {
      rc = Inter(SCVSEND, conprev->next, buf);

      if( rc != SCV_NO_ERR )
         rc = SCV_ERR + SUB_ERR - rc;
   }
   else
   {
      rc = SCV_ERR + SUB_ERR + rc;     /* format err from scv_qpth() */
   }
   s_drop( &tmpvmid );
 }
 else
 {
    rc = SCV_ERR + SUB_ERR + ERR_NEVER_INIT;     /* conhead never init */
 }

   return(rc);
}
