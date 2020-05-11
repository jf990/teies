/*********************************************************************
   <DOC>
   <NAME>    CRI_DUMP.C
   <DATE>    March 20, 1990
   <AUTHOR>  JF
   <PURPOSE> Dump out the Conference Root Index
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
#include "version.h"

extern void printf();



main( argc, argv, envp )


sint  argc;
byte      **argv;
byte             *envp;


{


  string        holder      = S_NULL,
                config_file = S_NULL,
                version     = S_NULL,
                spacer      = S_NULL,
                sys_id      = S_NULL,
                conf_label  = S_NULL,
                conf_id     = S_NULL,
                temp        = S_NULL;
  sint          goid        =      0,
                rc          =      0,
                num_entries =      0;
  hword         type        =      0,
                lfd         =      0;
  OID         * conf_oid    =    NIL,
              * cri_oid     =    NIL,
              * label_oid   =    NIL,
              * id_oid      =    NIL;
  OCB         * psi_ocb     =    NIL,
              * cri_ocb     =    NIL,
              * conf_ocb    =    NIL;
  OCD         * psi_ocd     =    NIL,
              * cri_ocd     =    NIL;
  boolean       more_keys   =   TRUE;






#D_main (  CRI_DUMP, TM  )



s_init( & version, VERSION_ID );


if( argc > 1 )
{

   #D_show( 'TEIES started with %d args','argc' );
   s_init( & spacer, " " );
   while( --argc )
   {

      ++argv;
      s_init( & holder, *argv );
      s_cat(  & config_file, config_file, holder, 0, 0 );
      s_cat(  & config_file, config_file, spacer, 0, 0 );

   }
}
else
{

   s_init( & config_file, "A * TEIES SYS" );

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

psi_ocb = t_sysndx( PSI_OBJ );
cri_oid = (OID *) os_mema( sizeof( OID ));
os_memn(  cri_oid,         sizeof( OID ));

s_init( & sys_id, "CRI" );

if( OCB_POINTER_OK( psi_ocb ) )
{

   psi_ocd = ocd_new( & psi_ocd, "PSI.INDEX.OID" );
   s_init( &(psi_ocd->key), "CRI" );
   rc = obj_get( psi_ocb, psi_ocd, cri_oid );
   if( rc != 0 )
   {

      #D_say( 'OID of the CRI not found - THERE IS NO CRI!' );
      #D_return( CRI_MISSING_OR_INVALID );

   }
   s_init( & (psi_ocd->name), "PSI.INDEX.LFD"  );
   obj_get(   psi_ocb,        psi_ocd, & lfd   );
   s_init( & (psi_ocd->name), "PSI.INDEX.GOID" );
   obj_get(   psi_ocb,        psi_ocd, & goid  );
   s_init( & (psi_ocd->name), "PSI.INDEX.TYPE" );
   obj_get(   psi_ocb,        psi_ocd, & type  );

   printf("+++++++++++++  FROM PSI  +++++++++++++++\n");
   printf("|CRI   :: LFD=%d; GOID=%d; ObjType=%d\n", lfd,goid,type );
   printf("|OID - %X %X %X %X %X %X %X %X\n", \
     cri_oid->one,  cri_oid->two, cri_oid->three, cri_oid->four, \
     cri_oid->five, cri_oid->six, cri_oid->seven, cri_oid->eight   );
   printf("+++++++++++++------------+++++++++++++++\n");

   cri_ocb = obj_opn( cri_oid, OBJ_READ );
   if( ! OCB_POINTER_OK( cri_ocb ) )
   {

      #D_leave( CRI_MISSING_OR_INVALID, 'Cannot open the CRI.' );

   }
   num_entries = obj_qns( cri_ocb, "CRI.ID" );
   printf("+++ Number of CRI Entries: %d  +++\n", num_entries);

   label_oid = (OID *) os_mema( sizeof( OID ));
   id_oid    = (OID *) os_mema( sizeof( OID ));
   os_memn(   label_oid,        sizeof( OID ));
   os_memn(   id_oid,           sizeof( OID ));

   ocd_new( & cri_ocd, NIL );
   os_memn(   cri_oid,       sizeof( OID ) );
   s_alloc( &(cri_ocd->key), sizeof( OID ) );
   os_memn( s_buffer( cri_ocd->key ), sizeof( OID ));
   while( more_keys )
   {

      s_init( &(cri_ocd->name), "CRI.ID.CONF_ROOT" );
      cri_ocd->bias = 1;
      rc = obj_get( cri_ocb, cri_ocd, id_oid );
      if( rc == NO_MORE_KEYS || rc == NO_DATA_FOUND )
      {

         more_keys = FALSE;
         printf("Tree exausted, keys are the same.\n");
         break;

      }
      s_copy( & conf_id, cri_ocd->key, 0, 0 );
      conf_ocb = obj_opn( id_oid, OBJ_READ );
      if( OCB_POINTER_OK( conf_ocb ) )
      {

         rc = obj_find( conf_ocb, "CONFERENCE.LABEL", & conf_label );

         s_copy( & (cri_ocd->key), conf_label, 0, 0 );
         cri_ocd->bias = 0;
         s_init( & (cri_ocd->name), "CRI.LABEL.CONF_ROOT" );

         rc = obj_get( cri_ocb, cri_ocd, label_oid );

         #D_show( 'RC from get of label OID is %d', 'rc' );

         printf( "========================================\n");
         s_put( conf_id,    '\0', s_len( conf_id )    + 1 );
         s_put( conf_label, '\0', s_len( conf_label ) + 1 );
         printf( "Conference ID    {%s}\n", s_buffer(conf_id)    );
         printf( "Conference Label {%s}\n", s_buffer(conf_label) );
         printf( "ID    OID - %X %X %X %X %X %X %X %X\n", \
            id_oid->one,  id_oid->two, id_oid->three, id_oid->four,\
            id_oid->five, id_oid->six, id_oid->seven, id_oid->eight);
         printf( "Label OID - %X %X %X %X %X %X %X %X\n", \
            label_oid->one, label_oid->two,label_oid->three,label_oid->four,\
            label_oid->five,label_oid->six,label_oid->seven,label_oid->eight);
         printf( "========================================\n");

         s_copy( & (cri_ocd->key), conf_id, 0, 0 );
         cri_ocd->bias = 1;
         s_init( & (cri_ocd->name), "CRI.ID.CONF_ROOT" );

      }
      else
      {

         #D_say( 'Indexed conference object cannot be opened.' );
         except( OID_IS_NOT_OPEN, FATAL_ERROR, 0, 0, 0, 0, 0 );

      }

   }
   rc = 0;
   ocd_drp( & cri_ocd );
   ocd_drp( & psi_ocd );
   obj_cls(   psi_ocb );

}
else
{

   rc = LFD_1_IS_MISSING;

}
os_memd( cri_oid   );
os_memd( id_oid    );
os_memd( label_oid );
                                          /*************************/
                                          /*                       */
                                          /* Shutdown the master   */
t_shutdn(  );                             /* process and make sure */
                                          /* proper cleanup is done*/
                                          /*                       */
                                          /*************************/


#D_return( rc )

}
