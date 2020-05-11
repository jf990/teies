/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_READ.C

:LEVEL.                OSK

:AUTHOR.               John L. Foster

:DATE.                 April 29, 1987

:INPUT.            device : Virtual device to read from. TEIES string.
                   buffer : A pointer to a buffer where the results
                            of the read are to go.
                   length : A pointer to an integer where on entry the
                            length of the buffer is contained. Upon
                            exit, this length will be updated to reflect
                            the number of bytes actually read.

:OUTPUT.           - see error codes below -

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.

              Calls the Assembly routine OSREAD to wait for a key
              interrupt from the terminal (virtual console) and
              once it occurs read the data stream from the terminal.
              The OSREAD function sets up the proper interrupts and
              waits for data to be ready. Upon entry a buffer and
              a pointer to its length are passed. On exit, both
              the buffer contents and the length are updated. The
              lenght will contain the number of bytes read.
:CALLS.
              inter   :  OSK
              osread  :  OSK

:ERRORS.
              14000   :  Read sucessful.
              14001   :  Read requested from a valid virtual device
                         but there is no real device assocaited to it.
              14002   :  Device characteristics have changed since last
                         operation.
              14028   :  Device not open.
              14032   :  Read requested, but another application has
                         taken control of the device. An Erase/Write(A)
                         must be preformed before the Read.
              14100   :  An I/O error has occured on the device while
                         the Read was in progress. Issue a Query and
                         decipher the problem.
:LIMITS.
           Length must point to an integer that is greater
           than two (2).
:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"




sint os_read(  vaddr, buffer, length )

string         vaddr;
byte                 *buffer;
sint                         *length;

{

sint     rc = 0;




    rc  =  Inter( OSREAD, buffer, length );

                                               /*********************/
                                               /*                   */
                                               /* With debugging on,*/
                                               /*                   */
                                               /*********************/



   return(14000+rc);
}
