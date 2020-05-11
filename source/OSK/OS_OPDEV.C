/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_OPDEV.C

:LEVEL.                OSK

:AUTHOR.               John L. Foster

:DATE.                 May  11, 1987

:INPUT.                device:       TEIES string containing the
                                     device name (address)
                       buffer:       A pointer to a data buffer
                                     to hold the Open device data.
                       len_buffer:   The length of the data buffer.

:OUTPUT.               rc:           Bubbled up from the Asm call.

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

         Call the operating system Assembly function OSOPDEV
         to perform the necessary open function on the device
         requested. A data buffer needs to be supplied for
         device dependent information pertanent to the device
         (to be) opened. The format of this buffer is:

          ?

:CALLS.            OSOPDEV:  OSK open device

:ERRORS.           14000  :  Device open properly.
                   14028  :  Device already open.
                   14001  :  Device open but there is no real
                              real device attached.
                   14040  :  Invalid device requested.
                   14088  :  Device requested cannot be used with
                              this function.
                   14104  :  Open request cannot not be processed
                              due to lack of storage.
:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"



sint     os_opdev(    device   )

string                device;


{

sint     rc             =   0,
         len_buffer     =   0;
tint     dev            =   0;
CQYSECT *buffer         = NIL;





    buffer=  (CQYSECT *) os_mema( len_buffer=sizeof(CQYSECT) );
    rc    =  s_stoti( &dev,  device, 0, 0  );

    if( dev == 0 )
    {
        dev = -1;
    }


    rc    =  Inter( OSOPCON,  dev,  buffer,  len_buffer );


                                         /* debugger switch 1 is on  */
             os_memd( buffer );


   return(14000 + rc);


}
