/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_len                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 19, 1988                                       |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object     OCB *      INPUT   Object the component     |
  |                                          belongs to.              |
  |                                                                   |
  |  <PARAM>   component  OCD *      INPUT   Component to determine   |
  |                                          length of.               |
  |                                                                   |
  |  <PURPOSE> To find the length of the OCD data.                    |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>  May have problems if ocdptr points to the last         |
  |            element in a segment                                   |
  |                                                                   |
  |  <LIMITS>  If the last element in the segment is a OC_STR, then   |
  |            its possible length cannot/is not computed.            |
  |                                                                   |
  |            String length computation does NOT include the 4 bytes |
  |            of overhead for the string used/allocated header.      |
  |                                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint ocd_len  ( object, component )

OCB *           object   ;
OCD *           component;


{


   ODT   * odt_entry           = NIL ;
   sint    length_of_component =   0 ;





if( OCB_POINTER_OK( object ) && OCD_POINTER_OK( component ) )
{

   odt_entry = odt_find( object, component ) ;

   if( ODT_POINTER_OK( odt_entry ) )
   {

      length_of_component = odt_len( odt_entry, object->type );

   }
}


   return(length_of_component);
}
