/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ocd_dcd                                                |
  |  <AUTHOR>  JF, RANA                                               |
  |  <DATE>    July 19, 1989                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  None                                                   |
  |  <PARAM>   *ocd_ptr  struct OCD   INOUT  Pointer to the OCD that  |
  |                                          will get the new OCD, key|
  |                                          and bias information.    |
  |                                                                   |
  |  <PURPOSE> Given an ocd structure with a name field in it, decode |
  |            the name field to determine the index key and bias     |
  |            information, if any exists.  The elements ->key and    |
  |            ->bias will be updated with the new data.  ->name will |
  |            be corrected to its proper OCD for ODT lookups.        |
  |                                                                   |
  |            Format of a user level Component is:                   |
  |                                                                   |
  |              Root.Component.Component(key,offset).component       |
  |                                                                   |
  |            Where the Root and components are concatentated into   |
  |            ocd->name,  key is removed and placed in ocd->key,     |
  |            and offset is converted to an integer and placed in    |
  |            ocd->bias.                                             |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <OUTPUT>  ocd_ptr->key, ocd_ptr->bias, ocd_ptr->name if data.    |
  |                                                                   |
  |  <CALLS>                                                          |
  |                                                                   |
  |  <ERRORS>                                                         |
  |                                                                   |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"

sint       ocd_dcd ( ocd_ptr   )


OCD        *ocd_ptr;


{

sint     rc             =  0,
         len            =  0,
         offset         =  0;

string   source         =  S_NULL,
         name           =  S_NULL,
         needle         =  S_NULL,
         garbage        =  S_NULL,
         dis_allowed_chars =  S_NULL,
         first          =  S_NULL,
         key            =  S_NULL,
         bias           =  S_NULL,
         temp_str       =  S_NULL,
         str_or_hex     =  S_NULL;


byte     *byte_ptr      =  NIL;





if( OCD_POINTER_OK ( ocd_ptr ) )
{

   s_copy( & source,   ocd_ptr->name, 0, 0 );
   s_upper( source, 0, 0 );

   s_init( & needle, "(" );
   len = s_first( needle, source, 0, 0 );
   if( len == 0 )
   {

      s_trim( & source, 'B' );
      s_copy( & first, source, 0, 0 );

   }
   else
   {
      s_clip( & first, & source, 0, len   );
      s_clip( & garbage, & first, -1, 1   );
      s_trim( & first, 'B' );


      s_init( & needle, "," );
      len = s_first( needle, source, 0, 0 );
      if( len == 0 )
      {

         s_init( & needle, ")" );
         len = s_first( needle, source, 0, 0 );
         if( len == 0 )
         {
            s_clip( & key, & source, 0, len );
         }
         else
         {
            s_clip( & key, & source, 0, len );
            s_clip( & garbage, & key, -1, 1 );
            s_trim( & key, 'B' );

         }

      }
      else
      {
         s_clip( & key, & source, 0, len     );
         s_clip( & garbage, & key, -1, 1     );
         s_trim( & key, 'B' );

         s_init( & needle, ")" );
         len = s_first( needle, source, 0, 0 );
         if( len == 0 )
         {
            s_clip( & bias, & source, 0, 0 );
            if( s_len( bias ) > 0 )
            {
               s_stosi( & offset, bias, 0, 0     );
            }

         }
         else
         {
            s_clip( & bias, & source, 0, len );
            s_clip( & garbage, & bias, -1, 1  );
            s_trim( & bias, 'B' );
            if( s_len( bias ) > 0 )
            {
               s_stosi( & offset, bias, 0, 0     );
            }
         }


      }

      if( s_len( source ) != 0 )
      {

         s_trim( & source, 'B' );
         s_cat ( &name, first, source, 0, s_len( source ) );
         s_copy( & first, name, 0, 0 );

      }

   }

   s_init( & dis_allowed_chars, " ,;\"()'" );
   rc = s_verify( first, dis_allowed_chars, 0, 0, S_DISALLOW );
   if( rc == 0 )
   {

      s_copy( & ocd_ptr->name, first, 0, 0 );
      ocd_ptr->bias = offset;

      /*-----------------------------------------------------*/
      /* So far we have ocd_ptr->name, and ocd_ptr->bias set */
      /* in place, now we have to determine whether the key  */
      /* data to be put into ocd_ptr->key is a string ( i.e  */
      /* if any quotes " " are provided ) or it is in hex    */
      /* form ( if no quotes are provided, if so proper con- */
      /* version has to be done                              */
      /*-----------------------------------------------------*/
      if(  s_len( key )  > 0 )
      {

         s_init( & str_or_hex, "\"" );
         rc = s_verify( key, str_or_hex, 0, 0, S_DISALLOW );
         if( rc == 0 )
         {
            s_alloc ( & ( ocd_ptr -> key ), 32 );
            byte_ptr = s_buffer ( ocd_ptr -> key );
            s_sxtom ( & byte_ptr, key, 0, 0 );
            s_smp_ul( ocd_ptr ->key ) = s_len ( key ) / 2 +
                                        s_len ( key ) % 2  ;

         }
         else
         {

            s_copy( & temp_str, key, 0, 0  );
            if( s_char_s( key, 0 ) == '\"' )
            {

               s_clip( &key, &temp_str, 2, s_len( key ) );

            }

            if( s_char_s( key, s_len( key ) - 1 ) == '\"' )
            {

               s_copy( &temp_str, key, 0, 0 );
               s_clip( &key, &temp_str, 0, s_len( key ) - 1  );

            }

            s_trim( & key, 'B' );
            s_copy( & ocd_ptr->key,  key,  0, 0 );
            rc = 0;

         }

      }
      else
      {

         s_copy( & ocd_ptr->key, key,  0,  0 );

      }

   }
   else
   {

      rc = INVALID_OCD_STRING ;

   }


}
else
{

   rc = OCD_POINTER_INVALID;

}

s_drop( & needle            );
s_drop( & dis_allowed_chars );
s_drop( & str_or_hex        );

   return(rc);

}
