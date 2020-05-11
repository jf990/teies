/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    mk_object()                                            |
  |  <AUTHOR>  John L. Foster,  Steve Muccione                        |
  |  <DATE>    June  1, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE> This program creates and saves on disk an ODT of type  |
  |            requested.                                             |
  |  <OUTPUT>  "objectodt" on disk                                    |
  |  <CALLS>   odt_disp (), odt_save (), odt_make()                   |
  |  <ERRORS>  None.                                                  |
  |  <LIMITS>  Blindly overwrites the file if it already exists.      |
  |  <MODS>    Apr 13 88         S. Muccione  Modified for use with   |
  |                                           ODT_MAKE.               |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "tm.h"
#include "teies.h"
#include "version.h"

extern sint odt_init();
extern void printf();






main( argc, argv, envp )
sint  argc;
byte      **argv;
byte             *envp;


{


    sint       rc          =      0,
               key_request =      0,
               connections =      0;
    string     holder      = S_NULL,
               config_file = S_NULL,
               version     = S_NULL,
               fname       = S_NULL,
               no_request  = S_NULL,
               senddata    = S_NULL,
               spacer      = S_NULL;
    OCB       *ocb         =    NIL;
    ODT       *odt_user    =   NULL;
    OCD       *ocd         =    NIL;
    hword      object_type =      0;



#D_main  ( MK_OBJECT,  OBJ  )





s_init( &version, VERSION_ID );
s_init( &no_request, "NONE"  );


if( argc > 1 )
{

   #D_show( 'TEIES started with %d args','argc' );
   s_init( &spacer, " " );
   while( --argc )
   {

      ++argv;
      s_init( &holder, *argv );
      s_cat(  &config_file, config_file, holder, 0, 0 );
      s_cat(  &config_file, config_file, spacer, 0, 0 );

   }
}
#D_str( 'holder','Requested Object' );
#D_say( 'Initializing TEIES' );
t_init( S_NULL, S_NULL, 0 );

if( s_len( holder ) < 1 )
{

    string field = S_NULL;


    #D_say( 'No object requested, asking for one.' );
    s_init( &field, "OBJNAME" );
    s_init( &no_request, "MAKEOBJ" );
    key_request = t_form( no_request, NIL, NIL );

    if( t_ifquit( key_request ) )
    {

       #D_leave( '0', 'User requested QUIT' );

    }

    s_copy( & holder, var_get( field ), 0, 0);
    s_drop( & field      );
    s_drop( & no_request );

}

s_trim( &holder, 'B'  );
s_upper( holder, 0, 0 );
#D_str( 'holder', 'Object name' );
s_init(&spacer, "* * " );
s_cat( &fname, spacer, holder, 0, 0 );
s_init(&spacer, " OBJECT" );
s_cat( &fname, fname, spacer, 0, 0 );




      ocb = obj_opn ( (OID *) fname, OBJ_READ | OS_FILE);
      if( ! OCB_POINTER_OK( ocb ) )
      {

         #D_say( 'File is bad.' );
         #D_str( 'fname', 'Bad file name' );
         #D_return( 'GFD_DOES_NOT_EXIST' );

      }
      ocd = ocd_new ( & ocd, NIL );

      #D_say( 'Files open, making object ODT' );
      odt_make (ocb,ocd);           /* Create ODT and display it       */
      #D_say( 'Object made, saved on disk. ');
      object_type = obj_inta( holder );
      odt_user = odt_ptr( object_type );

      if( ODT_POINTER_OK( odt_user ) )
      {

         odt_disp (odt_user);

      }

   s_drop(  & fname        );
   s_drop(  & config_file  );
   s_drop(  & spacer       );
   s_drop(  & version      );
   ocd_drp( & ocd          );
   t_shutdn(               );
   #D_exit (  0            );
}
