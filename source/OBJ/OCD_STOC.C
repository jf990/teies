/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_stoc()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 30, 1987                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>                                                         |
  |                                                                   |
  |  <PARAM>   *ocb     struct OCB  INPUT    Pointer to the Object's  |
  |                                          Control Block            |
  |            *ocd     struct OCD  INPUT    Pointer to the Object    |
  |                                          Component Descriptor to  |
  |                                          convert.                 |
  |             data    string      INPUT    String containing data   |
  |                                          to convert and place in  |
  |                                          the object.              |
  |                                                                   |
  |  <PURPOSE> This routine will convert the string into the proper   |
  |            object component data type and replace it in the object|
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   ocb_dbp (), odt_find (), dms_sdp (), dms_find (),      |
  |            dms_qd (), dms_qdl (), except ()                       |
  |  <ERRORS>  20023 : data requested was not found.                  |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"


sint   ocd_stoc( ocb, ocd, data )


OCB        *ocb        ;          /* Pointer to the OBJ control block  */
OCD        *ocd        ;          /* The Obj Component Descriptor      */
string      data       ;          /* String containing input data      */



{

   sint       rc     =   0    ;   /* Return code from functions        */
   byte      *buffer =   NIL  ;   /* Temp work buffer for data         */
   hword      ocdtype=   0    ;   /* Type of OCD to work with          */
   ODT       *odtptr =   NIL  ;   /* OCD definition                    */
   boolean    deallocated = FALSE;/*                                   */




   odtptr = odt_find( ocb, ocd );
   if( ODT_POINTER_OK( odtptr ) && s_len( data ) > 0 )
   {

      if( (ocdtype = odt_type( odtptr )) != OC_STR )
      {

         buffer = os_mema( 28 );
         os_memn(  buffer, 28 );

      }
      else
      {

         buffer = S_NULL;

      }

      switch (  ocdtype  )
      {

      case OC_BYTE:

         *(byte *)buffer = s_char_s( data, 1 );
         break ;


      case OC_HWRD:

         s_stoh( (hword *)buffer, data, 0, 0 );
         break ;


      case OC_INT :

         s_stosi( (sint *)buffer, data, 0, 0  );
         break ;


      case OC_REAL:

         s_stor( (real *)buffer, data, 0, 0 );
         break ;


      case OC_STR :

         os_memd( buffer );
         buffer = S_NULL;
         s_copy( (string *) & buffer, data, 0, 0 );
         deallocated = TRUE;
         break ;


      case OC_D_T :

         s_sxtom( & buffer, data, 0, 0 );
         break ;


      case OC_OID :

         s_sxtom( & buffer, data, 0, 0 );
         break ;


      case OC_PRIV:

         s_sxtom( & buffer, data, 0, 0 );
         break ;


      case OC_ISV :

         s_stoh( (hword *)buffer, data, 0, 0 );
         /* isv_match() */
         break ;


      default     :

         rc = INVALID_OCD_TYPE;
         except (   rc, INFO, data, S_NULL,S_NULL,S_NULL,S_NULL) ;
         break ;

      }
      if( rc == 0 )
      {

         rc = obj_rep( ocb, ocd, buffer, SEG_UPDATE );

      }
      if( ! deallocated )
      {

         os_memd( buffer );

      }
   }
   else
   {

      if( s_len( data ) < 1 )
      {

         rc = NO_DATA_PROVIDED;

      }
      else
      {

         rc = OCD_NOT_IN_ODT;

      }
   }


   return(rc);
}
