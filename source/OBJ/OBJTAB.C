/*********************************************************************

 OBJTAB.C      Test obj_inta()                June 13, 1988       JF


  :PURPOSE. Determine command line parameters, initialize the system,
            Signon, call homebase, and signoff.

  :PARAM.   argc    sint      INPUT  Total number of cmd line args +1
  :PARAM.   argv    byte **   INPUT  Pointer to array of strings
  :PARAM.   envp    byte **   INPUT  Pointer to environment variables

  :OUTPUT.  rc      sint             Any resulting error code.

  :CALLS.   t_init(), t_s_on(), t_s_off(), t_shutdn(), s_drop().

  :ERRORS.  none

  :LIMITS.
*********************************************************************/

#define D_OUTFILE "teies bugout B"
#define D_DUALOUT FALSE


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf.h"
#include "obj.h"
#include "form.h"
#include "tm.h"
#include "teies.h"
#include "version.h"
#include "debugi.h"

extern void printf();




main( argc, argv, envp )


sint  argc;
byte      **argv;
byte             *envp;


{


    sint       rc          =      0;
    string     holder      = S_NULL;
    string     config_file = S_NULL,
               version     = S_NULL,
               input_object= S_NULL,
               spacer      = S_NULL;
    hword      object_type =      0;


#D_main  ( OBJTAB,  OBJ )
#D_tron();


s_init( &version, VERSION_ID );


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
else
{

   s_init( &config_file, "* * TEIES SYS" );

}
#D_str( 'config_file','configuration file default' );


                                              /*************************/
                                              /* First we call TM init */
                                              /* to initialize the TM, */
                                              /* init TI, verify a MS  */
                                              /* will talk to this user*/
    t_init(config_file,                       /* and then initiate a   */
           version, RELEASE_ID );             /* logon.                */
    s_drop(&spacer);                          /*                       */
    s_drop(&config_file);                     /*                       */
    s_drop(&version);                         /*************************/

s_init( &input_object, "MAIL" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

s_init( &input_object, "CONFERENCE" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

s_init( &input_object, "CONF" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

s_init( &input_object, "SNI" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

s_init( &input_object, "JUNK" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

s_init( &input_object, "soidi" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

s_init( &input_object, "" );
object_type = obj_inta( input_object );
#D_str( 'input_object', 'Looking for ...' );
#D_show( 'Type is %d.', 'object_type');

#D_say( 'Test complete' );
                                              /*************************/
                                              /*                       */
                                              /* Shutdown the master   */
    t_shutdn(  );                             /* process and make sure */
                                              /* proper cleanup is done*/
                                              /*                       */
                                              /*************************/
#D_troff();
printf( "\n\n\n\nRun of Obj_inta() completed.\n\n\n" );
#D_return( 0 )

}
