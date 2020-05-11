/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_ini()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 23, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   ocb         OCB *      INPUT  OCB of the component.    |
  |  :PARAM.   ocd         OCD *      INPUT  Component descriptor.    |
  |                                                                   |
  |  :PURPOSE. Initialize an Object component to default contents.    |
  |            For Strings, blank out the data portion, set the       |
  |            used length to 0 and the allocated length to the ODT   |
  |            offset defined.  For all other components NULL out     |
  |            the data.                                              |
  |                                                                   |
  |  :OUTPUT.  sint error code.  0 if object ok, component defined    |
  |                 and obj_get() and obj_rep() worked.  Otherwise    |
  |                 resulting error messages are passed back.         |
  |                                                                   |
  |  :CALLS.   odt_sgln(), obj_get(), obj_rep(), os_memn(),           |
  |            s_nulout().                                            |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint   ocd_ini( ocb, ocd )

OCB            *ocb;
OCB            *ocd;


{

   sint    rc    =    0;





   return(rc);

}
