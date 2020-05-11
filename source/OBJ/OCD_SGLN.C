/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    ocd_sgln                                               |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 09, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   current   *ODT        INPUT   This is a pointer to an  |
  |                                          OCD which is contained   |
  |                                          in the segment whose len |
  |                                          is to be found.          |
  |                                                                   |
  |  :PARAM.   obj_type  hword       INPUT   Type of object to check. |
  |                                                                   |
  |  :PURPOSE. To find the length of the segment that the given OCD   |
  |            is a parent of.                                        |
  |                                                                   |
  |  :OUTPUT.  seglen   sint         The segment length.              |
  |                                                                   |
  |  :CALLS.   sizeof(), odt_ptr().                                   |
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





sint ocd_sgln ( current, obj_type )


ODT        *current;              /* Pointer to an OCD in an ODT       */
hword       obj_type;

{


   ODT        *ocdptr = NIL  ,    /* Ptr in an ODT that can be changed */
              *odt    = NIL  ;    /* Ptr in an ODT that can be changed */
   sint        seglen = 0    ;    /* Length of requested segment       */







if( odt_type( current ) >= OC_SEG && odt_type( current ) <= OC_MAX )
{


   ocdptr = ++current;            /*************************************/
   odt = odt_ptr( obj_type );     /* Step through the ODT starting at  */
                                  /* the position of the requested OCD,*/
                                  /* and locate the last entry in the  */
                                  /* segment that is not a segment type*/
                                  /*************************************/
   while ((odt_lvl(current)  == odt_lvl( odt + odt_nxto(current)))  &&
          (odt_off(current)  <  odt_off( odt + odt_nxto(current)))    )
   {


      current = odt + odt_nxto( current );

   }
                                  /* Get length of last element        */


   seglen = odt_off(  current );
   switch ( odt_type( current ))
   {
   case OC_BYTE:
      seglen += 1;
      break ;
   case OC_HWRD:
      seglen += 2;
      break ;
   case OC_INT :
      seglen += 4;
      break ;
   case OC_REAL:
      seglen += sizeof( real );
      break ;
   case OC_STR :
      seglen += 82;                    /* Tweek for now until ODT fixed*/
      break ;
   case OC_D_T :
      seglen += sizeof (datetime) ;
      break ;
   case OC_INHR:
   case OC_OID :
      seglen += sizeof (OID) ;
      break ;
   case OC_PRIV:
      seglen += sizeof (PRIVS) ;
      break ;
   default     :
      seglen += 0 ;
      break ;
   }
}




   return(seglen);



}
