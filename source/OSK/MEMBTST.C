
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf.h"
#include "obj.h"
#include "obj_memb.h"
#include "form.h"
#include "tm.h"
#include "teies.h"
#include "version.h"

main( argc, argv, envp )


sint  argc;
byte      **argv;
byte             *envp;


{


   OCB   *conf_ocb = NULL;

   sint   rc = NO_ERR;

   hword  level = 0;


    string     holder      = S_NULL;
    string     config_file = S_NULL,
               version     = S_NULL,
               dat_str     = S_NULL,
               spacer      = S_NULL;


   #D_main( membtst, OBJ)

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
    t_init(config_file,  version,             /* and then initiate a   */
                         RELEASE_ID );        /* logon.                */
    s_drop(&spacer);                          /*                       */
    s_drop(&config_file);                     /*                       */
    s_drop(&version);                         /*************************/

   conf_ocb = obj_all( CONF_OBJ, LOCAL_OBJECT, NULL );

   if( OCB_POINTER_OK( conf_ocb ) )
   {

      s_init( &dat_str, "Sal's ID");
      obj_update(conf_ocb, "CONFERENCE.ID", dat_str);

      s_init( &dat_str, "Sal's TITLE");
      obj_update(conf_ocb, "CONFERENCE.TITLE", dat_str);

      s_init( &dat_str, "Sal's LABEL");
      obj_update(conf_ocb, "CONFERENCE.LABEL", dat_str);

      rc = cr_memb( conf_ocb, S_NULL, level, NULL );

      s_drop( &dat_str );

   }

   obj_cls( conf_ocb );


   t_shutdn();

 #D_exit(0)

}
