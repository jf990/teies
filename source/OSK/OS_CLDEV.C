/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_CLDEV.C

:LEVEL.                OSK

:AUTHOR.               John L. Foster

:DATE.                 May  11, 1987

:INPUT.                device:       TEIES string containing the
                                     device name (address)

:OUTPUT.               rc:           Bubbled up from the Asm call.

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

         Call the operating system Assembly function OSCLCON
         to perform the necessary close function on the device
         requested.

:CALLS.            OSCLCON:  OSK open device

:ERRORS.

:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"



sint     os_cldev(    device    )

string                device;


{

sint     rc             = 0;





    rc    =  Inter( OSCLCON,  device );


   return(14000 + rc);


}
