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
         key_str        =  S_NULL,
         bias           =  S_NULL;



#D_begin(  ocd_dcd,  OBJ  );
#D_show( 'Pointer is x%x.', 'ocd_ptr' );
#D_str(  'ocd_ptr->name', 'OCD to decode.' );
#D_ocd( ocd_ptr );


if( OCD_POINTER_OK ( ocd_ptr ) )
{

   s_copy( & source,   ocd_ptr->name, 0, 0 );
   s_upper( source, 0, 0 );
   #D_str(  'source', 'The source str to be disected' );

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
      #D_str( 'first','What is in first'  );
      s_clip( & garbage, & first, -1, 1   );
      s_trim( & first, 'B' );
      #D_str( 'first', 'What is in first' );
      #D_str( 'source','What is in source');


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
         #D_str( 'key',   'What is in key'   );
         s_clip( & garbage, & key, -1, 1     );
         s_trim( & key, 'B' );
         #D_str( 'key',   'What is in key'   );
         #D_str( 'source','What is in source');

         s_init( & needle, ")" );
         len = s_first( needle, source, 0, 0 );
         if( len == 0 )
         {
            s_clip( & bias, & source, 0, 0 );
            if( s_len( bias ) > 0 )
            {
               #D_say( 'converting bias to sint' );
               s_stosi( & offset, bias, 0, 0     );
            }

         }
         else
         {
            s_clip( & bias, & source, 0, len );
            #D_str( 'bias', 'What is in bias' );
            s_clip( & garbage, & bias, -1, 1  );
            s_trim( & bias, 'B' );
            #D_str( 'bias', 'What is in bias' );
            if( s_len( bias ) > 0 )
            {
               #D_say( 'converting bias to sint' );
               s_stosi( & offset, bias, 0, 0     );
               #D_show( 'What is in offset = %d', 'offset' );
            }
            #D_str( 'source','What is in source');
         }


      }

      if( s_len( source ) != 0 )
      {

         s_trim( & source, 'B' );
         s_cat ( &name, first, source, 0, s_len( source ) );
         #D_str( 'name', 'What is in name' );
         #D_str( 'source','What is in source');
         s_copy( & first, name, 0, 0 );

      }

   }

   s_init( & dis_allowed_chars, " ,;\"()'" );
   #D_str( 'dis_allowed_chars', 'Chars which cannot be in ocd->name');
   #D_str( 'first', 'what is going to be ocd->name' );
   rc = s_verify( first, dis_allowed_chars, 0, 0, S_DISALLOW );
   #D_show( 'rc of s_verify = %d', 'rc' );

   if ( ( s_len ( key ) > 0  )  && ( s_char_s(key,0) != '"' ) )
   {
        byte_ptr = s_buffer ( key_str );
        s_sxtom  ( &byte_ptr,  key    ,  0, 0 );
        s_copy   ( & key    ,  key_str,  0, 0 );
   }
   if( rc == 0 )
   {

      s_copy( & ocd_ptr->name, first, 0, 0 );
      ocd_ptr->bias = offset;

      s_copy( & ocd_ptr->key,  key,  0, 0 );

   }
   else
   {

      rc = INVALID_OCD_STRING ;

   }

#D_ocd( ocd_ptr );

}
else
{

   rc = OCD_POINTER_INVALID;

}

s_drop( & needle            );
s_drop( & dis_allowed_chars );

#D_leave( rc, 'if zero OK, otherwise problem' );

}
