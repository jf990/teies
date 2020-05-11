/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    gt_object()                                            |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June  1, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :PURPOSE. This program displays an ODT that is already on disk.  |
  |                                                                   |
  |  :OUTPUT.  "objectodt" on disk                                    |
  |                                                                   |
  |  :CALLS.   odt_disp (), odt_save (), odt_make()                   |
  |                                                                   |
  |  :ERRORS.  None.                                                  |
  |                                                                   |
  |  :LIMITS.  Blindly overwrites the file if it already exists.      |
  |                                                                   |
  |  :MODS.    Apr 13 88         S. Muccione  Modified for use with   |
  |                                           ODT_MAKE.               |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/




#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "osk_i.h"
#include "obj.h"
#include "vars.h"
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
               connections =      0;
    string     holder      = S_NULL;
    string     config_file = S_NULL,
               version     = S_NULL,
               fname       = S_NULL,
               no_request  = S_NULL,
               senddata    = S_NULL;
    OCB       *ocb         =    NIL;
    ODT       *odt_user    =   NULL;
    hword      object_type =      0;



#D_main  ( GT_OBJECT,  OBJ  )





s_init( &version, VERSION_ID );
s_init( &no_request, "NONE"  );


if( argc > 1 )
{

   #D_show( 'TEIES started with %d args','argc' );
   while( --argc )
   {

      ++argv;
      s_init( &holder, *argv );
      s_cat(  &config_file, config_file, holder, 0, 0 );

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
    holder = t_form( no_request, NIL, NIL );
    s_copy( &holder, var_get( field ), 0, 0);
    s_drop( &field );
    s_drop( &no_request );

}

s_trim( &holder, 'B'  );
s_upper( holder, 0, 0 );
#D_str( 'holder', 'Object name' );





      object_type = obj_inta(  holder  );
      odt_user    = odt_ptr( object_type );

      if( odt_user != NIL && odt_user != -1 )
      {

         odt_disp (odt_user);

      }

   s_drop( & fname       );
   s_drop( & config_file );
   s_drop( & version     );
   t_shutdn(             );
   #D_exit (  0          );
}
