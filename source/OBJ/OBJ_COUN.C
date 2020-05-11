/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_count()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    March 1, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT  Object to use            |
  |  <PARAM>   ocd         OCD *      INPUT  Component to locate      |
  |                                                                   |
  |  <PURPOSE> Return the number of segments under a component.       |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   s_len(), s_buffer(), ocd_dbp(), dms_sdp(), dms_qns().  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"



sint   obj_count(  ocb, ocd  )

OCB               *ocb;
OCD               *ocd;

{

   sint        rc             =      0,
               segment_count  =      0;
   string      data_base_path = S_NULL;




if( OCB_POINTER_OK( ocb ) && OCD_POINTER_OK( ocd ) )
{

   rc = ocd_dbp( ocb, ocd, & data_base_path );
   if( rc == 0 )
   {

      rc = dms_sdp( ocb->dmscb, s_buffer( data_base_path ),
                                s_len(    data_base_path ) );
      segment_count = dms_qns( ocb->dmscb );

   }
   s_drop( & data_base_path );

}

   return(segment_count);

}
