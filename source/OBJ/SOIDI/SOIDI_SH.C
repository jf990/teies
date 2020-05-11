/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    SOIDI_SHOW()                                           |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    May 20, 1988                                           |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Display the contents of the SOIDI with debug statements|
  |            by opening the PSI object and step through each entry  |
  |            in it and print the Key, OID, LFD, GOID, and object    |
  |            type.                                                  |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   os_memc(), dms_obj(), dms_ce(), dms_sd(), dms_sdp()    |
  |            odt_ptr(), dms_cls(), oid_new()                        |
  |            except(),  obj_rep(), obj_get()                        |
  |                                                                   |
  |                                                                   |
  |  :ERRORS.  20997 : PSI does not exist.                            |
  |                                                                   |
  |  :LIMITS.                                                         |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "dms.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"




sint   soidi_show(  )



{



OID          *psi_oid    =   NIL,
             *soidi_oid  =   NIL,
             *oid        =   NIL;
OCB          *psi_ocb    =   NIL,
             *soidi_ocb  =   NIL;
OCD          *psi_ocd    =   NIL,
             *soidi_ocd  =   NIL;
boolean       more_keys  =  TRUE;
sint          rc         =     0,
              goid       =     0;
hword         type       =     0,
              lfd        =     0;
string        sys_id     =S_NULL;



#D_begin(  soidi_show, OBJ      );

    psi_oid = (OID *) os_mema( sizeof( OID ));
    soidi_oid=(OID *) os_mema( sizeof( OID ));
    os_memn( psi_oid, sizeof( OID ) );
    os_memn(soidi_oid,sizeof( OID ) );
    #D_oid( psi_oid, 'PSI' );

            /* = = = = = = = = = = = = = = = = = = = = = = = = = = = =*
             *                                                        *
             * Locate the PSI by allocating an OID of all 0's. Once   *
             * the PSI is open we can find the SOIDI.                 *
             *                                                        *
             * = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

    oid     = (OID *) os_mema( sizeof( OID ));
    os_memn( oid,     sizeof( OID ) );
    #D_oid( psi_oid, 'PSI' );
    s_init(&sys_id, "SOIDI" );

    psi_ocb = obj_opn( psi_oid, OBJ_READ );
    if( OCB_POINTER_OK( psi_ocb ) )
    {

       psi_ocd = ocd_new( &psi_ocd, "PSI.INDEX.OID" );
       s_init( &(psi_ocd->key), "SOIDI" );
       rc = obj_get( psi_ocb, psi_ocd, soidi_oid );
       if( rc != 0 )
       {

          #D_say( 'OID of the SOIDI not found - THERE IS NO SOIDI!' );
          #D_return(-1);

       }
       s_init( &(psi_ocd->name), "PSI.INDEX.LFD" );
       obj_get( psi_ocb, psi_ocd, &lfd  );
       s_init( &(psi_ocd->name), "PSI.INDEX.GOID" );
       obj_get( psi_ocb, psi_ocd, &goid );
       s_init( &(psi_ocd->name), "PSI.INDEX.TYPE" );
       obj_get( psi_ocb, psi_ocd, &type );

       #D_say('++++++++++++++++++++++++++++++++++++++++');
       #D_show('SOIDI :: LFD=%d; GOID=%d; ObjType=%d','lfd,goid,type');
       #D_oid( 'soidi_oid', 'SOIDI' );
       #D_say('++++++++++++++++++++++++++++++++++++++++');

       soidi_ocb = obj_opn( soidi_oid, OBJ_READ );
       if( ! OCB_POINTER_OK( soidi_ocb ) )
       {

          #D_leave( -1, 'Cannot open the SOIDI.' );

       }

            /* = = = = = = = = = = = = = = = = = = = = = = = = = = = =*
             *                                                        *
             * The SOIDI has instance variables (OCD's) OID as the key*
             * and LFD, GOID and Type similar to PSI.                 *
             *                                                        *
             * = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

       os_memn( &(soidi_oid), sizeof( OID ) );
       ocd_new( &soidi_ocd, NIL );
       s_alloc( &(soidi_ocd->key), sizeof( OID) );
       os_memn( s_buffer( soidi_ocd->key ), sizeof( OID ));
       while( more_keys )
       {

          s_init( &(soidi_ocd->name), "SOIDI.INDEX.OID" );
          soidi_ocd->bias = 1;
          if ( 0 == (rc = obj_get( soidi_ocb, soidi_ocd,  oid  )))
          {

             s_init( &(soidi_ocd->name), "SOIDI.INDEX.LFD" );
             obj_get( soidi_ocb, soidi_ocd, &lfd  );
             s_init( &(soidi_ocd->name), "SOIDI.INDEX.GOID" );
             obj_get( soidi_ocb, soidi_ocd, &goid );
             s_init( &(soidi_ocd->name), "SOIDI.INDEX.TYPE" );
             obj_get( soidi_ocb, soidi_ocd, &type );

             #D_say( '(=======================================?');
             #D_show('Type=%d; LFD=%d; GOID=%d.','type,lfd,goid');
             #D_oid( 'oid', 'OID Entry'                         );
             #D_say( '(=======================================?');

          }

          if( rc == NO_MORE_KEYS )
          {

             more_keys = FALSE;
             #D_say('Tree exausted, keys are the same.');

          }

       }
       rc = 0;
       ocd_drp( &soidi_ocd );
       ocd_drp( &psi_ocd   );

    }
    else
    {

       rc = 20997;

    }
    os_memd( psi_oid  );
    os_memd( soidi_oid);
    os_memd( oid      );

#D_return( rc );

}
