/*********************************************************************
  <Doc>
  <Name>    OBJ_TEST
  <Author>  JF
  <Date>    March 21, 1989
  <Level>   OBJ
  <PURPOSE> Set up the teies environment and test some OBJ routines.

  <PaRAM>   argc    sint      INPUT  Total number of cmd line args +1
  <PARAM>   argv    byte **   INPUT  Pointer to array of strings
  <PARAM>   envp    byte **   INPUT  Pointer to environment variables

  <OUTPUT>  rc      sint             Any resulting error code.
  <CALLS>
  <ERRORS>  none
  <LIMITS>
  </DOC>
*********************************************************************/



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
               packet      = S_NULL,
               spacer      = S_NULL;

    OCB      * ocb         =    NIL;
    OCD      * ocd         =    NIL;
    OID        oid;
    sint       data;


#D_main  ( OBJ_TEST, OBJ  )


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




ocb = obj_all( MEMBERSHIP_OBJ, LOCAL_OBJECT, NIL );

ocd_new( & ocd, "MEMBERSHIP.MEMBERS.STATUS" );
oid_key( ocd, &(ocb->oid) );

data = 99;
obj_rep( ocb, ocd, & data, SEG_UPDATE );

ocd_new( & ocd, "MEMBERSHIP.MEMBERS" );
ocd->bias = 1;

obj_get( ocb, ocd, & oid );

s_init( &(ocd->name), "MEMBERSHIP.MEMBERS.STATUS" );
ocd->bias = 0;
data = 0;
obj_get( ocb, ocd, & data );

#D_oid( '(&oid)', '' );
#D_show( 'Data %d', 'data' );

ocd_drp( & ocd );
obj_cls( ocb );

                                              /*************************/
                                              /*                       */
                                              /* Shutdown the master   */
    t_shutdn(  );                             /* process and make sure */
                                              /* proper cleanup is done*/
                                              /*                       */
                                              /*************************/


#D_return( 'rc' );

}
