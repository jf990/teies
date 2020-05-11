/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    os_gfdd.c                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    march 16, 1988                                         |
  |                                                                   |
  |  :LEVEL.   OSK                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   gfd         string     INPUT   Generalized file name   |
  |                                           of the file to be erased|
  |                                                                   |
  |  :PURPOSE. Erase (remove) an operating system file, given the     |
  |            TEIES GFD of the file to erase.                        |
  |                                                                   |
  |  :OUTPUT.  sint.  0 if erased, otherwise OSK level error.         |
  |                                                                   |
  |  :CALLS.   unlink(), os_qfd(), s_init(), s_drop()                 |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"

extern sint unlink();




sint   os_gfdd( gfd )

string          gfd;


{


sint      rc               =      0;
string    default_gfd      = S_NULL,
          system_file_name = S_NULL;





  s_init( &default_gfd, "* * * *" );
  os_qfd( &system_file_name, gfd, default_gfd );
  s_put(   system_file_name, NULL, s_len( system_file_name ) + 1 );

  rc = unlink( s_buffer( system_file_name) );
  if( rc != 0 )
  {

     rc += 10000;

  }
  s_drop( &system_file_name );
  s_drop( &default_gfd      );


   return(rc);

}
