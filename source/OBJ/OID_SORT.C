/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    oid_sort()                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    October 24, 1988                                       |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.                                                          |
  |                                                                   |
  |  :PURPOSE. Sort a list of OIDs by the data in a given Component.  |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.                                                          |
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



sint   oid_sort(  oid_array, component_to_sort_by, length_of_array,flags)

OID             **oid_array;
OCD              *component_to_sort_by;
sint              length_of_array,
                  flags;



{

   sint    rc          =         0;
   OID     oid_holder             ;





   return(rc);

}
