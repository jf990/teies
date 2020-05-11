/*+-------------------------------------------------------------------+*/
/*|  <DOC>                                                            |*/
/*|  <NAME>    s_sxtom.c                                              |*/
/*|  <AUTHOR>  John L. Foster                                         |*/
/*|  <DATE>    May 31, 1989                                           |*/
/*|  <LEVEL>   STR                                                    |*/
/*|  <GLOBAL>                                                         |*/
/*|  <PARAM>   target      byte  ** OUTPUT   destination of conversion|*/
/*|  <PARAM>   source      string   INPUT    hex character digits.    |*/
/*|  <PARAM>   beg         sint     INPUT    where to begin in source |*/
/*|  <PARAM>   len         sint     INPUT    length to convert.       |*/
/*|                                                                   |*/
/*|  <PURPOSE> Convert hexidecimal character digits in the source str |*/
/*|            into binary pointer to by target.  Conversion will stop|*/
/*|            if a non-hex character is encountered in the source.   |*/
/*|                                                                   |*/
/*|  <OUTPUT>  sint                                                   |*/
/*|  <CALLS>   s_copy(), s_len(), s_upper(), s_char_s()               |*/
/*|  <ERRORS>                                                         |*/
/*|  <LIMITS>  If the memory pointer appears to be valid but is not   |*/
/*|            a protection or addressing exception may occur.        |*/
/*|  <MODS>                                                           |*/
/*|  </DOC>                                                           |*/
/*+-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"



sint s_sxtom ( target, source, beg, len )

byte        ** target ;
string         source ;
sint           beg    ,
               len    ;

{


   string  hex_string        = S_NULL;
   sint    result_length     =      0,
           position          =      0,
           rc                =      0;
   byte    hex_char          =     '0',
           hex_byte          =      0;
   boolean high_order_nibble =   TRUE;




   if( target != NIL && target != (byte **) -1 )
   {

      s_copy( & hex_string, source, beg, len );
      if( s_len( hex_string ) > 0 )
      {

         if( * target == NIL )
         {

            * target = (byte *) os_mema( ( s_len( hex_string ) / 2 ) + 1 );
         }
         s_upper( hex_string, 0, 0 );

         for( position = 0; position < s_len( hex_string ); position ++ )
         {

            hex_char = s_char_s( hex_string, position );

            if( hex_char >= '0' && hex_char <= '9' )
            {

               hex_byte += ( hex_char - '0' ) ;

            }
            else if ( hex_char >= 'A' && hex_char <= 'F' )
            {

               hex_byte += ( hex_char - 'A' + 10 ) ;

            }
            else
            {

               break;

            }

            if( high_order_nibble == TRUE )
            {

               hex_byte = hex_byte * 0x10 ;
               high_order_nibble = FALSE;

            }
            else
            {

               * ( (* target ) + (position / 2) ) = hex_byte ;
               high_order_nibble = TRUE;
               hex_byte = 0;
               result_length ++ ;

            }
         }


      }
      else
      {


      }
      s_drop( & hex_string );

   }
   else
   {


   }



   return(rc);
}
