/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_isnull()                                           |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 22, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |                                                                   |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   object      OCB *      INPUT  Object to get data from. |
  |  <PARAM>   component   OCD *      INPUT  Component to check.      |
  |                                                                   |
  |  <PURPOSE> Return TRUE if the data of the component is NULL.      |
  |                                                                   |
  |  <OUTPUT>  boolean                                                |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"

#define MAXIMUM_DATA_LENGTH 28


boolean    obj_isnull(  object, component  )

OCB                   * object;
OCD                   * component;

{

   boolean     rc          =  FALSE;

   byte      * null_data   =    NIL,
             * data_buffer =    NIL;

   string      string_data = S_NULL;

   sint        result      =      0,
               data_length =      0;

   ODT       * odt_entry   =    NIL;





if( OCB_POINTER_OK( object ) )
{

   if( OCD_POINTER_OK( component ) )
   {




      odt_entry = odt_find( object, component );

      if( odt_type( odt_entry ) == OC_STR )
      {

         result = obj_get( object, component, & string_data );

         if(     ( result == NO_DATA_FOUND  )
            || ( ( result == 0              )
            &&   ( s_len( string_data ) < 1 ) ))
         {

            rc = TRUE ;

         }

         s_drop( & string_data );

      }
      else if ( odt_type( odt_entry ) < OC_SEG )
      {

         data_buffer = os_mema( MAXIMUM_DATA_LENGTH );
         null_data   = os_mema( MAXIMUM_DATA_LENGTH );
         os_memn(  data_buffer, MAXIMUM_DATA_LENGTH );
         os_memn(  null_data,   MAXIMUM_DATA_LENGTH );

         result = obj_get( object, component, data_buffer );

         if     ( result == NO_DATA_FOUND  )
         {

            rc = TRUE ;

         }
         else if ( result == 0 )
         {

            data_length = odt_len( odt_entry, object->type );

            result = os_mcmp( data_buffer, null_data, data_length );

            if( result == 0 )
            {

               rc = TRUE ;

            }
         }

         os_memd( null_data   );
         os_memd( data_buffer );

      }
   }
}

   return(rc);

}
