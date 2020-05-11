/*********************************************************************

  PSIDUMP.C    Main TEIES startup routine     August 22, 1986     JF


  :PURPOSE. Dump out the Principle Systems Index

  :PARAM.   argc    sint      INPUT  Total number of cmd line args +1
  :PARAM.   argv    byte **   INPUT  Pointer to array of strings
  :PARAM.   envp    byte **   INPUT  Pointer to environment variables

  :OUTPUT.  rc      sint             Any resulting error code.

  :CALLS.   t_init(), t_s_on(), t_s_off(), t_shutdn(), s_drop().

  :ERRORS.  none

  :LIMITS.
*********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "isf.h"
#include "obj.h"
#include "tm.h"
#include "ti.h"
#include "teies.h"
#include "version.h"

extern void printf();



main( argc, argv, envp )


sint  argc;
byte      **argv;
byte             *envp;


{


    sint       rc          =     0,
               i           =     0,
               goid        =     0;
    string     holder      = S_NULL,
               sys_id      = S_NULL,
               config_file = S_NULL,
               version     = S_NULL,
               spacer      = S_NULL;
  OID          *psi_oid    =   NIL,
               *oid        =   NIL;
  OCB          *psi_ocb    =   NIL;
  OCD          *psi_ocd    =   NIL;
  boolean       more_keys  =  TRUE;
  hword         type       =     0,
                lfd        =     0;
  byte          key_string =   NIL;



#D_main  ( PSIDUMP, TM    )



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

   s_init( &config_file, "A * TEIES SYS" );

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

                                              /*************************/
                                              /*                       */
                                              /* Display the PSI       */
/*  psi_dump();  */                           /*                       */
                                              /*                       */
                                              /*************************/

    printf("\n\n");
    psi_oid = (OID *) os_mema( sizeof( OID ));
    os_memn( psi_oid, sizeof( OID ) );
    #D_oid( psi_oid, 'PSI' );
    oid     = (OID *) os_mema( sizeof( OID ));
    os_memn( oid,     sizeof( OID ) );
    #D_oid( oid, 'PSI' );
    s_init(&sys_id, "                      L" );

    psi_ocb = obj_opn( psi_oid, OBJ_READ );
    if( psi_ocb != NIL || psi_ocb != -1 )
    {

       psi_ocd = ocd_new( &psi_ocd, NIL );
       while( more_keys )
       {

          s_init( &(psi_ocd->name), "PSI.INDEX.OID" );
          psi_ocd->bias = 1;

          rc = obj_get( psi_ocb, psi_ocd, oid  );
          printf( "rc = %d\n", rc );
          if( rc == 0 )
          {

             psi_ocd->bias = 0;
             s_init( &(psi_ocd->name), "PSI.INDEX.LFD"  );
             obj_get( psi_ocb, psi_ocd,           &lfd  );
             s_init( &(psi_ocd->name), "PSI.INDEX.GOID" );
             obj_get( psi_ocb, psi_ocd,           &goid );
             s_init( &(psi_ocd->name), "PSI.INDEX.TYPE" );
             obj_get( psi_ocb, psi_ocd,           &type );

             printf( "(=======================================?\n");
             printf( "Length of key is %d;  ", s_len( psi_ocd->key ) );
             printf( "Index is " );
             for( i=0; i<s_len(psi_ocd->key); i++ )
             {

                printf( "%c", s_char_s( psi_ocd->key, i ) );

             }
             printf( "\n" );
             printf("Type=%d; LFD=%d; GOID=%d\n",type,lfd,goid);
             printf("OID - %X %X %X %X %X %X %X %X\n", \
                    oid->one, oid->two, oid->three, oid->four,\
                    oid->five, oid->six, oid->seven, oid->eight) ;
             printf("(=======================================?\n");

          }
          else
          {

             more_keys = FALSE;
             printf("Tree exausted, keys are the same.\n\n");

          }



       }
       rc = 0;
       ocd_drp( &psi_ocd );

    }
    else
    {

       rc = LFD_1_IS_MISSING;

    }

                                              /*************************/
                                              /*                       */
                                              /* Shutdown the master   */
    t_shutdn(  );                             /* process and make sure */
                                              /* proper cleanup is done*/
                                              /*                       */
                                              /*************************/


#D_return( rc )

}
