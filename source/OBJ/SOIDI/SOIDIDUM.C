/*********************************************************************
   <DOC>
   <NAME>    SOIDIDUMP.C
   <DATE>    August 22, 1986
   <AUTHOR>  JF
   <PURPOSE> Dump out the System Object IDentifier Index
   <PARAM>   argc    sint      INPUT  Total number of cmd line args +1
   <PARAM>   argv    byte **   INPUT  Pointer to array of strings
   <PARAM>   envp    byte **   INPUT  Pointer to environment variables
   <OUTPUT>  rc      sint             Any resulting error code.
   <CALLS>   t_init(), t_s_on(), t_s_off(), t_shutdn(), s_drop().
   </DOC>
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
#include "debugi.h"
#include "version.h"

extern void printf();



main( argc, argv, envp )


sint  argc;
byte      **argv;
byte             *envp;


{


    string     holder      = S_NULL;
    string     config_file = S_NULL,
               version     = S_NULL,
               spacer      = S_NULL;
  boolean       more_keys  =  TRUE;
  sint          goid       =     0,
                rc         =     0,
                num_entries=     0;
  hword         type       =     0,
                lfd        =     0;
  string        sys_id     =S_NULL,
                temp       =S_NULL;
  OID          *soidi_oid  =   NIL,
               *oid        =   NIL;
  OCB          *psi_ocb    =   NIL,
               *soidi_ocb  =   NIL;
  OCD          *psi_ocd    =   NIL,
               *soidi_ocd  =   NIL;






#D_main  ( SOIDIDUM, TM  )



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
    lfd  = 1;                                 /*                       */
    goid = 2;                                 /* Display the SOIDI as- */
    dms_pg( lfd, goid );                      /* suming it is GOID 2.  */
                                              /*                       */
                                              /*************************/
    soidi_oid = (OID *) os_mema( sizeof( OID ) );
    os_memn(    soidi_oid,       sizeof( OID ) );

            /* = = = = = = = = = = = = = = = = = = = = = = = = = = = =*
             *                                                        *
             * Locate the PSI by allocating an OID of all 0's. Once   *
             * the PSI is open we can find the SOIDI.                 *
             *                                                        *
             * = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

    s_init( & sys_id, "SOIDI" );

    psi_ocb = t_sysndx( PSI_OBJ );
    if( OCB_POINTER_OK( psi_ocb ) )
    {

       psi_ocd = ocd_new( &psi_ocd, "PSI.INDEX.OID" );
       s_init( &(psi_ocd->key), "SOIDI" );
       rc = obj_get( psi_ocb, psi_ocd, soidi_oid );
       if( rc != 0 )
       {

          #D_say( 'OID of the SOIDI not found - THERE IS NO SOIDI!' );
          #D_return( 'SOIDI_MISSING_OR_INVALID' );

       }
       s_init( &(psi_ocd->name), "PSI.INDEX.LFD" );
       obj_get( psi_ocb, psi_ocd, &lfd  );
       s_init( &(psi_ocd->name), "PSI.INDEX.GOID" );
       obj_get( psi_ocb, psi_ocd, &goid );
       s_init( &(psi_ocd->name), "PSI.INDEX.TYPE" );
       obj_get( psi_ocb, psi_ocd, &type );

       printf("++++++++++++++++++++++++++++++++++++++++\n");
       printf("SOIDI :: LFD=%d; GOID=%d; ObjType=%d\n", lfd,goid,type );
       printf("OID - %X %X %X %X %X %X %X %X\n", \
       soidi_oid->one,soidi_oid->two,soidi_oid->three,soidi_oid->four,\
       soidi_oid->five,soidi_oid->six,soidi_oid->seven,soidi_oid->eight);
       printf("++++++++++++++++++++++++++++++++++++++++\n");

       soidi_ocb = t_sysndx( SOIDI_OBJ );
       if( ! OCB_POINTER_OK( soidi_ocb ) )
       {

          #D_leave( SOIDI_MISSING_OR_INVALID, 'Cannot open the SOIDI.' );

       }
       num_entries = obj_qns( soidi_ocb, "SOIDI.INDEX" );
       printf("+++ Number of SOIDI Entries:  %d  +++\n", num_entries);

            /* = = = = = = = = = = = = = = = = = = = = = = = = = = = =*
             *                                                        *
             * The SOIDI has instance variables (OCD's) OID as the key*
             * and LFD, GOID and Type similar to PSI.                 *
             *                                                        *
             * = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

       oid     = (OID *) os_mema( sizeof( OID ));
       ocd_new( & soidi_ocd, NIL );
       os_memn( soidi_oid, sizeof( OID ) );
       s_alloc( &(soidi_ocd->key), sizeof( OID) );
       os_memn( s_buffer( soidi_ocd->key ), sizeof( OID ));
       while( more_keys )
       {

          s_init( &(soidi_ocd->name), "SOIDI.INDEX.OID" );
          soidi_ocd->bias = 1;
          rc = obj_get( soidi_ocb, soidi_ocd, oid );
          if( rc == NO_MORE_KEYS || rc == NO_DATA_FOUND )
          {

             more_keys = FALSE;
             printf("Tree exausted, keys are the same.\n");
             break;

          }

          lfd = 0;
          soidi_ocd->bias = 0;
          s_init( &(soidi_ocd->name), "SOIDI.INDEX.LFD" );
          obj_get( soidi_ocb, soidi_ocd, &lfd  );
          goid = 0;
          s_init( &(soidi_ocd->name), "SOIDI.INDEX.GOID" );
          obj_get( soidi_ocb, soidi_ocd, &goid );
          type = 0;
          s_init( &(soidi_ocd->name), "SOIDI.INDEX.TYPE" );
          obj_get( soidi_ocb, soidi_ocd, &type );

          printf( "========================================\n");
          printf( "Type=%d; LFD=%d; GOID=%d.\n", type,lfd,goid);
          printf("OID - %X %X %X %X %X %X %X %X\n", \
                  oid->one,  oid->two, oid->three, oid->four,\
                  oid->five, oid->six, oid->seven, oid->eight);
          printf( "========================================\n");

       }
       rc = 0;
       ocd_drp( &soidi_ocd );
       ocd_drp( &psi_ocd   );

    }
    else
    {

       rc = LFD_1_IS_MISSING;

    }
    os_memd( soidi_oid);
    os_memd( oid      );
                                              /*************************/
                                              /*                       */
                                              /* Shutdown the master   */
    t_shutdn(  );                             /* process and make sure */
                                              /* proper cleanup is done*/
                                              /*                       */
                                              /*************************/


#D_return( rc )

}
