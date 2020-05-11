/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    os_fcpy()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    July 7, 1989                                           |
  |  <LEVEL>   OSK                                                    |
  |  <GLOBAL>  None                                                   |
  |                                                                   |
  |  <PARAM>   source_gfd  string     INPUT  File to copy.            |
  |  <PARAM>   target_gfd  string     INPUT  File to copy to.         |
  |                                                                   |
  |  <PURPOSE> Copy operating system file source_gfd to operating     |
  |            system file target_gfd.  Both are specified in TEIES   |
  |            GFD format.                                            |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   os_call(), os_qfd()                                    |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"




sint   os_fcpy( source_gfd, target_gfd )

string          source_gfd, target_gfd ;


{

   sint        rc          =      0;
   string      default_gfd = S_NULL,
               source_file = S_NULL,
               target_file = S_NULL,
               os_function = S_NULL,
               parameters  = S_NULL,
               blank       = S_NULL;





if( s_len( source_gfd ) > 0 && s_len( target_gfd ) > 0 )
{

   os_qfd( & source_file, source_gfd, default_gfd );
   os_qfd( & target_file, target_gfd, default_gfd );

   s_init( & blank,       " "                            );
   s_init( & os_function, "COPYFILE "                    );
   s_cat(  & os_function, os_function, source_file, 0, 0 );
   s_cat(  & os_function, os_function, blank,       0, 0 );
   s_cat(  & os_function, os_function, target_file, 0, 0 );
   s_init( & parameters,  " ( APPEND NEWDATE"            );
   s_cat(  & os_function, os_function, parameters,  0, 0 );


   rc = os_call( os_function );

   s_drop( &   default_gfd );
   s_drop( &   source_file );
   s_drop( &   target_file );
   s_drop( &   os_function );
   s_drop( &   parameters  );
   s_drop( &   blank       );

}
else
{

   rc = 1 ;

}



   return(rc);

}
