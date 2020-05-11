/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
:NAME.        OS_CON.C

:LEVEL.       OSK

:AUTHOR.      John L. Foster

:DATE.        June 11, 1987

:INPUT.  vaddr    : A TEIES string containing the Virtual Device
                      address to communicate with.
         buffer   : A pointer to the buffer of data to write to the
                      device OR a pointer to a buffer where date that
                      is read from the device is to be placed.
         length   : A pointer to an integer. For a write operation
                      this value is used to determine how large the
                      buffer is and therefore, how many bytes to write
                      to the device. For a read operation this indicates
                      the maximum size of the input buffer. Upon return
                      from the read, the value is updated to indicate
                      the actual number of bytes read.
         op       : The operation to be performed. Currently:
                                 0 : Write
                                 1 : Read
                                 x : no-op
:OUTPUT. rc       : The error return status code, see below.
         length   : The number of bytes read for a read operation.

:INPUT_GLOBALS.- none -

:OUTPUT_GLOBALS.- none -

:PURPOSE.

    Execute a write or read using a CCW channel program.             *
    A small amout on memory will be allocated to be used
    to contain the start of the CCW program and chained to the
    buffer passed. This chain is in turn sent to the device channel.

:CALLS.    INTER()   :  C to assembly interface
           OSCON()   :  Assembly version that really does the work.

:ERRORS.   14000     :  I/O sucessful.
           14001     :  The virtual device is open but there is
                        no real device associated to it.
           14002     :  The device characteristics have changed
                        since the last operation on this device,
                        so a Query operation (OSQYCON) should
                        be performed to determine the problem.
           14024     :  Conflicting request options.
           14028     :  The device requested is unknown to the
                        system. Possibly it was not opened
                        (OSOPCON).
           14032     :  I/O error on device. Issue (OSQYCON) to
                        receive information about the CCW program
                        and channel status.
           14033     :  The operation requested is undefined.
:LIMITS.
         Currently this works only for remote Virtual Consoles
         that are really not terminals but TEIES servers of
         various kinds out in the network.

         The length CANNOT be larger than 64K. No check is
         done to see if it is, so truncation will occur
         as it is cast from sint to hword.

:MODS.
    Author    Date      Description

    JF        06/11/87  Completely re-wrote the routine.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "debug.h"
/* #include "ibm3270.h"  */


sint     os_con    ( vaddr, buffer, length, op )

string               vaddr;
byte                       *buffer;
sint                               *length;
sint                                        op;


{

sint        rc  =   0;
CCW     *header = NIL;

#D_start(os_con,OSK,'Op=%d; Buffer=0x%x; Len=%d.',\
                    'op,    buffer,     *length' );

                                    /********************************/
    switch( op )                    /*                              */
    {                               /* Build the Channel program    */
                                    /* based on the operation req'd */
         case 1 :  {                /********************************/

                        header         = os_mema( sizeof(CCW) );
                        header->ccwcode=   0x29;
                        header->dataddr= buffer;
                        header->flags  =   0x40;
                        header->ctl    =   0x00;
                        header->count  = length;

                        rc             = Inter(OSCON,header,length);

                   }
         case 2 :  {

                        header         = os_mema( sizeof(CCW) );
                        header->ccwcode=   0x2A;
                        header->dataddr= buffer;
                        header->flags  =   0x40;
                        header->ctl    =   0x00;
                        header->count  = length;

                        rc             = Inter(OSCON,header,length);

                   }
         default : {

                        #D_show('Undefined operation %d', 'op' );

                   }
    }



#D_return(  '14000 + rc'  );

}
