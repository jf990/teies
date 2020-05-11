/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    PSI_DUMP.C                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    January 8, 1987                                        |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Display the contents of the PSI in debugging statements|
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




sint   psi_dump(  )



{



OID          *psi_oid    =   NIL,
             *oid        =   NIL;
OCB          *psi_ocb    =   NIL;
OCD          *psi_ocd    =   NIL;
boolean       more_keys  =  TRUE;
sint          rc         =     0,
              goid       =     0;
hword         type       =     0,
              lfd        =     0;
string        sys_id     =S_NULL;



#D_begin(  psi_dump, OBJ      );

    psi_oid = (OID *) os_mema( sizeof( OID ));
    os_memn( psi_oid, sizeof( OID ) );
    #D_oid( 'psi_oid', 'PSI' );
    oid     = (OID *) os_mema( sizeof( OID ));
    os_memn( oid,     sizeof( OID ) );
    #D_oid( 'psi_oid', 'PSI' );
    s_init(&sys_id, "                      L" );

    psi_ocb = obj_opn( psi_oid, OBJ_READ );
    if( OCB_POINTER_OK( psi_ocb ) )
    {

       psi_ocd = ocd_new( &psi_ocd, NIL );
       while( more_keys )
       {

          s_init( &(psi_ocd->name), "PSI.INDEX.SYS_ID" );
          psi_ocd->bias = 1;
          rc = obj_get( psi_ocb, psi_ocd, &sys_id    );
          s_init( &(psi_ocd->name), "PSI.INDEX.OID" );
          obj_get( psi_ocb, psi_ocd, oid  );
          s_init( &(psi_ocd->name), "PSI.INDEX.LFD" );
          obj_get( psi_ocb, psi_ocd,&lfd  );
          s_init( &(psi_ocd->name), "PSI.INDEX.GOID" );
          obj_get( psi_ocb, psi_ocd,&goid );
          s_init( &(psi_ocd->name), "PSI.INDEX.TYPE" );
          obj_get( psi_ocb, psi_ocd,&type );

          #D_say( '(=======================================?');
          #D_str(  'psi_ocd->key', 'System Index name.'  );
          #D_show('Type=%d; LFD=%d; GOID=%d.','type,lfd,goid');
          #D_oid( 'oid', 'From PSI'                          );
          #D_say( '(=======================================?');

          if( rc == NO_MORE_KEYS )
          {

             more_keys = FALSE;
             #D_say('Tree exausted, keys are the same.');

          }

       }
       rc = 0;
       ocd_drp( &psi_ocd );

    }
    else
    {

       rc = LFD_1_IS_MISSING;

    }

#D_return( rc );

}
