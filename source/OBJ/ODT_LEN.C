/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    odt_len                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 1, 1988                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  * none *                                               |
  |  <PARAM>   odt_entry   ODT *      INPUT  A pointer to an entry in |
  |                                          the ODT of the component |
  |                                          we need the length of.   |
  |                                                                   |
  |  <PARAM>   obj_type     hword     INPUT  Type of object.          |
  |                                                                   |
  |  <PURPOSE> Return the length of the data of a given component     |
  |            given a pointer to the entry in the ODT.               |
  |  <OUTPUT>  sint                          Length of data.          |
  |  <CALLS>                                                          |
  |  <ERRORS>  * none *                                               |
  |  <LIMITS>  Cannot handle bad pointers or improperly formatted     |
  |            ODT's.                                                 |
  |  <END>                                                            |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"



sint odt_len  ( odtptr, obj_type )

ODT            *odtptr ;
hword           obj_type;


{



   sint        component_length = 0    ;





   switch(   odtptr->type   )
   {

      case OC_BYTE:

         component_length = 1;
         break ;

      case OC_HWRD:
      case OC_ISV :

         component_length = 2;
         break ;

      case OC_INT :

         component_length = 4;
         break ;

      case OC_REAL:

         component_length = 4;
         break ;

      case OC_STR :

         component_length = (odtptr + 1)->offset - odtptr->offset - S_OVERHEAD;

         if( component_length > 3996 || component_length < 1 ||
             odtptr->level != (odtptr+1)->level )
         {

            component_length = 132;

         }
         break ;

      case OC_D_T :

         component_length = sizeof (datetime) ;
         break ;

      case OC_INHR:
      case OC_OID :

         component_length = sizeof (OID) ;
         break ;

      case OC_PRIV:

         component_length = sizeof (PRIVS) ;
         break ;

      default     :

         if( odt_type( odtptr ) >= OC_SEG && odt_type( odtptr ) <= OC_MAX )
         {

            component_length = ocd_sgln( odtptr, obj_type );

         }
         else
         {

            component_length = 0;

         }
         break ;

   }




   return(component_length);
}
