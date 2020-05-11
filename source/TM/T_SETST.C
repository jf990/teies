/********************************************************************

    T_SETST.C                   January 16,1987                JF

                                                        LEVEL: TM

    This function initializes the TEIES Transaction Manager State Lookup
    table for us with the TM and functions t_getst, t_nxtst, t_addst.

    FUNCTIONS:

         t_setst   Initialize the TM State Table.

    INPUT:

                   -  none  -

    OUTPUT:


                   TM State Table structure updated (array of strings).

    ERRORS:

         48050     Not enough memory for TM State Table.

    GLOBALS:

         TMstate   Global structure for the TM State Table.

    Related Routines:

         t_addst() Add a state index to the table.
         t_getst() Return a pointer to the entry code for a given
                   TM state entry.
         t_nxtst() Find the next state for a given state.

    Remarks:

              **  C H A N G E    L O G  **

   Date                Author                 Description

 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"



sint     t_setst(  )


{



sint     rc   =    0;







   return(rc);
}
