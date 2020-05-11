/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.                 OS_WRITE.C

:LEVEL.                OSK

:AUTHOR.               John L. Foster

:DATE.                 May 16, 1987

:INPUT.            device: TEIES string of device to write to.

                   buffer: Pointer to a buffer containing screen data.

                   length: Number of bytes of screen data.

                   flag  : Operation to preform

:OUTPUT.           - see error codes below -

:INPUT_GLOBALS.    - none -

:OUTPUT_GLOBALS.   - none -

:PURPOSE.
              Calls the one of three Assembly routines to perform
              a write operation to the device in question. The
              routine called depends on the value of the flag parameter.
              Flag values are currently:

                             0    Clear screen before write (E/W)
                             1    Establish alternate screen mode
                                  for the device and then perform
                                  the write operation (EWA)
                             x    Do not clear screen and overlay
                                  the existing screen with new data.

:CALLS.
              inter   :  OSK
              oswrite :  OSK

:ERRORS.
              14000   :  Write sucessful.
              14001   :  Write requested from a valid virtual device
                         but there is no real device assocaited to it.
              14002   :  Device characteristics have changed since last
                         operation.
              14028   :  Device not open.
              14032   :  Write requested, but another application has
                         taken control of the device. An Erase/Write(A)
                         must be preformed before the Read.
              14100   :  An I/O error has occured on the device while
                         the Write was in progress. Issue a Query and
                         decipher the problem.
:LIMITS.

:MODS.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"

                                                /*********************/
                                                /*                   */
                                                /*   Routine   and   */
sint os_write(  vaddr, buffer, length,  flag )  /*                   */
                                                /*  parameter        */
string          vaddr;                          /*                   */
byte                  *buffer;                  /*  definitions      */
sint                           length;          /*                   */
sint                                    flag;   /*                   */
                                                /*                   */
{                                               /*********************/

sint     rc = 0;





    switch(  flag  )
    {

         case 0    :    {


                             rc = Inter(OSCLRCON,buffer,length);


                             break;

                        }
         case 1    :    {


                             rc = Inter(OSEWACON,buffer,length);


                             break;

                        }
         default   :    {


                             rc = Inter(OSWRITE, buffer,length);

                             break;


                        }

    }

   return(14000 + rc);
}
