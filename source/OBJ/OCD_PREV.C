/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_prev.c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 11, 1988                                      |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   ocb   OCB *           INPUT   OCB of the object to     |
  |                                          find next OCD in.        |
  |  :PARAM.   ocd   OCD **          OUTPUT  OCD to update with next  |
  |                                          OCD.                     |
  |  :PARAM.   data  byte *          OUTPUT  Pointer to data buffer to|
  |                                          place OCD contents.      |
  |                                                                   |
  |  :PURPOSE. Given the object, return the PREVIOUS OCD from         |
  |            the current OCD for that object.  If there is no curnt,|
  |            return the last.   Returns both the OCD and the content|
  |            of that OCD.  If the OCD pointer in NIL, a new OCD is  |
  |            created.  If data is NIL, a data buffer is allocated.  |
  |            If these pointers are not NIL, -1, then they are assum-|
  |            ed valid.                                              |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  0 : Operation completed as expected.                   |
  |            20452 : Already positioned at the first OCD.           |
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

#define NO_MORE_OCDs 20451




sint   ocd_prev( ocb, ocd, data )

OCB             *ocb;
OCD                 **ocd;
byte                      *data;



{





   return(0);

}
