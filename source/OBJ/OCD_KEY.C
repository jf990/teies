/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_key C                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    March 26, 1988                                         |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   odt_ptr     ODT *      INPUT  Pointer to the component |
  |                                          entry in the ODT.        |
  |                                                                   |
  |  :PURPOSE. Determine if the component in question is a key to an  |
  |            indexed segment.                                       |
  |                                                                   |
  |  :OUTPUT.  boolean.  TRUE if component is a key field.            |
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



boolean     ocd_key( ocd_ptr )

odtrecord           *ocd_ptr;


{


   boolean  rc = FALSE;





   /*+-----------------------------------------------------------------+
     | Follow the ODT back until we hit an entry with an offset of 0.  |
     | this will indicate the beginning of the segment portion.  If the|
     | ODT entry immediately before the one we find with offset 0 is   |
     | of type NDX, then we are in a key.  Otherwise this was not a    |
     | component of an indexed segment key.                            |
     +-----------------------------------------------------------------+*/

   if(  ODT_POINTER_OK( ocd_ptr ) )
   {

      while( (odt_off( ocd_ptr ) != 0)  &&  (odt_type( ocd_ptr ) < OC_SEG ))
      {

         ocd_ptr--;

      }
      if( odt_type( ocd_ptr - 1 ) == OC_NDX )
      {

         rc = TRUE;

      }
   }


   return(rc);

}
