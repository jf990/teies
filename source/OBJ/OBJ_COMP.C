/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_compare()                                          |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 22, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   Object_a    OCB *      INPUT                           |
  |  <PARAM>   Component_a OCD *      INPUT                           |
  |  <PARAM>   Object_b    OCB *      INPUT                           |
  |  <PARAM>   Component_b OCD *      INPUT                           |
  |                                                                   |
  |  <PURPOSE> Compare the data of one component with that of another.|
  |            Return TRUE if they are binary similar.                |
  |                                                                   |
  |  <OUTPUT>  boolean                                                |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"




boolean       obj_compare( object_a, component_a, object_b, component_b )

OCB         * object_a,    * object_b;
OCD         * component_a, * component_b;

{

   boolean     rc         =  FALSE;

   string      string_data_a = S_NULL,
               string_data_b = S_NULL;

   byte      * data_buffer_a =    NIL,
             * data_buffer_b =    NIL;

   sint        result_a      =      0,
               result_b      =      0,
               length_to_compare =  0;

   hword       data_type_a   =      0,
               data_type_b   =      0;

   ODT       * odt_entry     =    NIL;




if( OCB_POINTER_OK( object_a ) && OCB_POINTER_OK( object_b ) )
{

   if( OCD_POINTER_OK( component_a ) && OCD_POINTER_OK( component_b ) )
   {

      odt_entry = odt_find( object_a, component_a );

      if( (data_type_a = odt_type( odt_entry )) == OC_STR )
      {

         result_a = obj_get( object_a, component_a, & string_data_a );
         if( result_a == 0 )
         {

            length_to_compare = s_len( string_data_a );
            data_buffer_a  = s_buffer( string_data_a );

         }
      }
      else if ( odt_type( odt_entry ) < OC_SEG )
      {

         data_buffer_a = os_mema( 28 );
         result_a = obj_get( object_a, component_a, data_buffer_a );
         if( result_a == 0 )
         {

            length_to_compare = odt_len( odt_entry, object_a->type );

         }
      }

      odt_entry = odt_find( object_b, component_b );

      if( (data_type_b = odt_type( odt_entry )) == OC_STR )
      {

         result_b = obj_get( object_b, component_b, & string_data_b );
         data_buffer_b = s_buffer( string_data_b );

      }
      else if ( odt_type( odt_entry ) < OC_SEG )
      {

         data_buffer_b = os_mema( 28 );
         result_b = obj_get( object_b, component_b, data_buffer_b );

      }

      if( result_a == 0 && result_b == 0 )
      {

         if( os_mcmp( data_buffer_a, data_buffer_b, length_to_compare ) == 0 )
         {

            rc = TRUE ;

         }
      }
      else if ( result_a == NO_DATA_FOUND && result_b == NO_DATA_FOUND )
      {

         rc = TRUE ;

      }

      if( data_type_a == OC_STR )
      {

         s_drop( & string_data_a );

      }
      else
      {

         os_memd( data_buffer_a );

      }
      if( data_type_b == OC_STR )
      {

         s_drop( & string_data_b );

      }
      else
      {

         os_memd( data_buffer_b );

      }

   }
}


   return(rc);

}
