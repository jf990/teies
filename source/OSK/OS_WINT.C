/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    os_wint()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    March 21, 1988                                         |
  |                                                                   |
  |  :LEVEL.   OSK                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   event       sint       Input   Type of event to wait   |
  |                                           for an interrupt on.    |
  |                                                                   |
  |  :PARAM.   interval    sint       Input   Amount of time to wait  |
  |                                           before interrupt does   |
  |                                           not occur.              |
  |                                                                   |
  |  :PURPOSE. Wait for a specific interrupt event.  This will place  |
  |            the server in an interrupt wait state with the OS.  If |
  |            the event does not occur in a specified peroid of time |
  |            then just return normally.  The interval parameter is  |
  |            specified in 100ths of a second.                       |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.   WAIT4INT()                                             |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  Currently the event type is ignored.  Will wait for any|
  |            interrupt and then just return.                        |
  |                                                                   |
  |            The interval period is also currently ignored by WAIT4.|
  |            However, os_wint() performs range checking on it anyway|
  |                                                                   |
  |            For VM/CMS, must have SET TIMER REAL.                  |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk_i.h"
#include "osk.h"
#include "debug.h"


sint   os_wint(  event, interval )

sint             event, interval;




{

sint    rc   =   0;





if( interval < 1 ) { interval = 1; }

if( interval > 43200 ) { interval = 43200; }

rc = Inter( WAIT4INT, interval );



   return(rc);

}
