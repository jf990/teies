/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    smi_get()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 11, 1988                                      |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   mail_oid    OID **     INPUT  OID of a mail object to  |
  |                                          get from the System Mail |
  |                                          Index (SMI).             |
  |                                                                   |
  |  :PARAM.   mail_id     string     INPUT  Id of the mail to use as |
  |                                          a unique index key.      |
  |                                                                   |
  |  :PARAM.   mail_label  string     INPUT  A unique label for the   |
  |                                          Mail also used to index  |
  |                                          it.                      |
  |                                                                   |
  |  :PURPOSE. Return a Mail Object ID indexed in the SMI given either|
  |            a Mail Item ID or a Mail Item Label.  ID search takes  |
  |            precedence.                                            |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.   obj_opn(), t_sysndx(), ocd_new(), ocd_drp(), obj_rep().|
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



sint   smi_get(  mail_oid, mail_id, mail_label  )

OID            **mail_oid;
string           mail_id, mail_label;


{


   OCD     * mail_ocd          =      NIL;
   OCB     * smi_ocb           =      NIL;
   sint      rc                =        1,
             mail_count        =        0;




if( mail_oid != NIL && mail_oid != (OID **) 0xFFFFFFFF )
{

   if ( ! OID_POINTER_OK( (*mail_oid) ) )
   {

      *mail_oid = ( OID * ) os_mema( sizeof( OID ) );
      os_memn( *mail_oid, sizeof( OID ) );

   }

   smi_ocb = t_sysndx( SMI_OBJ );

   if ( OCB_POINTER_OK( smi_ocb ) )
   {

      ocd_new( & mail_ocd,  NIL );

      if( s_len( mail_id ) > 0 )
      {

         s_init( & ( mail_ocd->name ), "SMI.MESSAGE.OID" );
         s_copy( & ( mail_ocd->key  ), mail_id, 0, 0     );
         rc = obj_get( smi_ocb, mail_ocd, * mail_oid     );

      }
      else if(  ( rc != 0 )  &&  ( s_len( mail_label ) > 0 )  )
      {

         s_init( & ( mail_ocd->name ), "SMI.LABEL.OID"  );
         s_copy( & ( mail_ocd->key  ), mail_label, 0, 0 );
         rc = obj_get( smi_ocb, mail_ocd, * mail_oid    );

      }

      ocd_drp( &    mail_ocd           );

   }
   else
   {

      rc = SMI_MISSING_OR_INVALID;

   }
}
else
{

   rc = INVALID_OID;

}

   return(rc);

}
