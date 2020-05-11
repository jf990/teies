/********************************************************************

    T_ATTRIB.C                October 22, 1986                 JF

                                                        LEVEL: TI

         Return the attributes of a "virtual" device for use
         by user interface (TM or Interpreter) functionality.



    FUNCTIONS:

         t_attrib  Return device attributes.



    INPUT:

         device    String describing the device.


    OUTPUT:


                 Returns a pointer to a list of strings describing
                   the device attributes.

    ERRORS:



    GLOBALS:





    Related Routines:



    Remarks:




              **  C H A N G E    L O G  **

   Date                Author                 Description




 ********************************************************************/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"



sint     t_attrib(  device  )



string     device;



{



#D_begin ( t_attrib , TM )


#D_return( 0 )

}
