/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_QYDEV.C

:LEVEL.                OSK

:AUTHOR.               John L. Foster

:DATE.                 May 16, 1987

:INPUT.            device: TEIES string of device to write to.

                   Buffer: Pointer to a buffer to place Query Results.
                           Must be 112 bytes in length.

:OUTPUT.           - see error codes below -

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

              Calls the Assembly routine OSQYCON to query the device
              and return a buffer of device dependent info.
              The device should have already been open before,
              and the buffer must be able to accept up to 112
              bytes. The buffer is mapped to the CQYSECT data
              structure defined in OSK.H and on page 29 of
              IBM SC24-5333-0.

:CALLS.
              inter   :  OSK

:ERRORS.      14000   :  Query processed OK.
              14028   :  Device requested not yet open.
              14040   :  Invalid device requested.
:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"




sint os_qydev(  device, buffer  )

string          device;
CQYSECT                *buffer;

{

    sint     rc = 0;


    rc = Inter(  OSQYCON, buffer, sizeof( CQYSECT)  );

                                           /***************************/
                                           /* Dump the Query results  */
                                           /* if debug switch 1 is on */
                                           /***************************/



   return(14000 + rc);
}
