/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    smi_qry()                                              |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 11, 1988                                      |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   flag    HWORD     INPUT    What to do.                 |
  |  :PURPOSE. Return the number of Mail Items in the SMI.            |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.              t_sysndx(), ocd_new(), ocd_drp(), obj_rep().|
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



sint   smi_qry(  flag  )

hword            flag;


{


   OCB     * smi_ocb           =      NIL;
   sint      rc                =        0,
             mail_count        =        0;




if( TM_master->usrtype == MASTER_SERVER )
{

   smi_ocb = t_sysndx( SMI_OBJ );

   if ( OCB_POINTER_OK( smi_ocb ) )
   {

      switch( flag )
      {

         case  0               :
         case  MAIL_ITEM_TOTAL :

               rc = obj_find( smi_ocb, "SMI.MAIL_COUNT", & mail_count );
               break;

         case  MAIL_ITEM_NEXT  :

               rc = obj_find( smi_ocb, "SMI.NEXT_AVAILABLE",&mail_count);
               break;

         default               :

               break;

      }
   }
   else
   {

      rc = SMI_MISSING_OR_INVALID;

   }
}
else
{


}
   return(mail_count);

}
