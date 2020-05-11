/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    smi_add()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 11, 1988                                      |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   mail_oid    OID *      INPUT  OID of a mail object to  |
  |                                          add to the System Mail   |
  |                                          Index (SMI).             |
  |                                                                   |
  |  :PARAM.   mail_id     string     INPUT  Id of the mail to use as |
  |                                          a unique index key.      |
  |                                                                   |
  |  :PARAM.   mail_label  string     INPUT  A unique label for the   |
  |                                          Mail also used to index  |
  |                                          it.                      |
  |                                                                   |
  |  :PARAM.   flags       hword      INPUT  Flag to determine type of|
  |                                          mail to index, either    |
  |                                                                   |
  |                         MAIL_ITEM_REPLY  Mail is a reply.         |
  |                         MAIL_ITEM_ROOT   Mail is a root.          |
  |                                                                   |
  |  :PURPOSE. Index a piece of mail in the SMI given an ID and Label.|
  |                                                                   |
  |  :OUTPUT.  sint                          If positive, the number  |
  |                                          of mail items in the SMI |
  |                                          If negative, an OBJ level|
  |                                          error.                   |
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
#include "obj_mail.h"



sint   smi_add(  mail_oid, mail_id, mail_label, flags  )

OID             *mail_oid;
string           mail_id, mail_label;
hword            flags;


{


   OCD     * mail_ocd          =      NIL;
   OCB     * smi_ocb           =      NIL;
   sint      rc                =        0,
             mail_root         =        0,
             mail_count        =        0;




if( OID_POINTER_OK( mail_oid ) )
{

   smi_ocb = t_sysndx( SMI_OBJ );

   if ( OCB_POINTER_OK( smi_ocb ) )
   {

      ocd_new(    & mail_ocd,  NIL );

      if( s_len( mail_id ) > 0 )
      {

         s_init( & ( mail_ocd->name ), "SMI.MESSAGE.OID"       );
         s_copy( & ( mail_ocd->key  ), mail_id, 0, 0           );
         rc = obj_rep( smi_ocb, mail_ocd, mail_oid, SEG_UPDATE );

      }
      if(  ( rc == 0 )  &&  ( s_len( mail_label ) > 0 )  )
      {

         s_init( & ( mail_ocd->name ), "SMI.LABEL.OID"           );
         s_copy( & ( mail_ocd->key  ), mail_label, 0, 0          );
         rc = obj_rep( smi_ocb, mail_ocd, mail_oid,   SEG_UPDATE );

      }
      if ( rc == 0 )
      {

         s_init( & ( mail_ocd->name ), "SMI.MAIL_COUNT"  );
         rc = obj_get( smi_ocb,   mail_ocd, & mail_count );

         if( rc != 0 )
         {

            mail_count = 0 ;

         }

         mail_count ++ ;
         rc = obj_rep( smi_ocb, mail_ocd, & mail_count, SEG_UPDATE );

         if ( flags == MAIL_ITEM_ROOT )
         {

            s_init( & ( mail_ocd->name ), "SMI.NEXT_AVAILABLE" );
            rc = obj_get( smi_ocb,   mail_ocd, & mail_root  );

            if( rc != 0 )
            {

               mail_root  = 0 ;

            }

            mail_root ++ ;
            rc = obj_rep( smi_ocb, mail_ocd, & mail_root, SEG_UPDATE );

         }
      }

      ocd_drp( &    mail_ocd           );
      obj_set_flag( smi_ocb, OBJ_FLUSH );
      obj_cls(      smi_ocb            );

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


if ( rc != 0 )
{

   rc = - rc;

}
else
{

   rc = mail_count;

}

   return(rc);

}
