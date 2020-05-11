/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_isvar()                                            |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    September 28, 1988                                     |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   *ocdptr    odtrecord  INPUT   This is an OCD pointer.  |
  |                                                                   |
  |  :PARAM.   obj_type   hword      INPUT   Type of object the ocdptr|
  |                                          belongs to.              |
  |                                                                   |
  |  :PURPOSE. To determine of the Component is actually a variable   |
  |            length segment.  This is true when the Component is    |
  |            of type string and is the LAST element in the segment. |
  |            Then the length of the component is the maximum size   |
  |            size the segment can ever grow to, but the used amount |
  |            of the string will be the ultimate segment length.     |
  |                                                                   |
  |  :OUTPUT.  Boolean                  TRUE  if a variable length seg|
  |                                     FALSE if not variable length  |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  May have problems if ocdptr points to the last         |
  |            element in a segment                                   |
  |                                                                   |
  |  :LIMITS.  If the last element in the segment is a OC_STR, then   |
  |            its possible length cannot/is not computed.            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



boolean ocd_isvar( odtptr, obj_type )

ODT               *odtptr  ;
hword              obj_type;


{



   boolean     rc = FALSE ;





   if( ( odt_type( odtptr )             == OC_STR                       )
    && ( odt_previous_offset( odtptr )  >= odt_next_offset( odtptr )    )
    && ( (odtptr - odt_ptr( obj_type )) >  odt_previous_offset( odtptr ))
     )
   {

      rc = TRUE;

   }




   return(rc);
}
