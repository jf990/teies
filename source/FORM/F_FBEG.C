/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    f_fbeg                                                 |
  |                                                                   |
  |  :AUTHOR.  Nestor R. Voronka                                      |
  |                                                                   |
  |  :DATE.    October 12, 1987                                       |
  |                                                                   |
  |  :LEVEL.   FORMS                                                  |
  |                                                                   |
  |  :GLOBAL.  name        type      OUTPUT  description              |
  |                                                                   |
  |  :PARAM.   name        type      OUTPUT  description              |
  |                                                                   |
  |  :PURPOSE. Locate and open the object containing the Form to parse|
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |                                                                   |
  |  :CALLS.   obj_opn(), t_sysndx(), ocd_new(), s_cat(), s_init().   |
  |  :ERRORS.  None.                                                  |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :MODS.                                                           |
  |            28_Nov_88    JF       Added conditional compile around |
  |                                  SFI logic since that does not    |
  |                                  work yet.  Also added proper mem-|
  |                                  ory deallocation at the end.     |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "ti.h"
#include "tm.h"
#include "debug.h"
#include "form_i.h"

sint f_fbeg (form_id, flags, form_ocb, form_ocd)
string       form_id  ;           /* Form's ID to look up in SFI       */
hword        flags    ;           /* Control flags from form           */
OCB        **form_ocb ;           /* Obj Cont Blk of opened form       */
OCD        **form_ocd ;           /* OBj Comp Desc of the forms text   */
{
   OCB        *sfi_ocb  =   NULL ;/* OCB of the System Forms Index     */
   OID        *form_oid =   NULL ;/* OID of the requested form         */
   OCD        *sfi_ocd  =   NULL ;/* OCD used with the SFI             */
   sint        rc       =      0 ;/* Return code from the OBJ level    */
   string      new_id   = S_NULL ;
   string      catstr   = S_NULL ;


#if 0

                                  /* Get OCB of System Forms Index     */
/* sfi_ocb = t_sysndx (SFI_OBJ) ;
  */                              /* Create proper Obj Comp Descriptor */
   sfi_ocd = ocd_new (&sfi_ocd, "SFI.INDEX.OID") ;
   s_copy (&(sfi_ocd->key), form_id, 0, 0) ;
                                  /* Create space for the Forms OID    */
   form_oid = (OID *) os_mema (sizeof (OID)) ;
                                  /* Get the OID from the SFI          */
/* rc = obj_get (sfi_ocb, sfi_ocd, form_oid) ;
  */                             /* Check the return code from obj_get */
   if (rc != 0)
   {
      ocd_drp( &sfi_ocd ) ;
   return(rc);
   }
                                  /* Create proper Obj Comp Descriptor */
   sfi_ocd = ocd_new (&sfi_ocd, "SFI.INDEX.OCD") ;
   s_copy (&(sfi_ocd->key), form_id, 0, 0) ;
                                  /* Create space for the Forms OCD    */
   *form_ocd = ocd_new( form_ocd, S_NULL );
                                  /* Get the OCD from the SFI          */
/* rc = obj_get (sfi_ocb, sfi_ocd, &( (*form_ocd)->name ) ) ;
  */                             /* Check the return code from obj_get */
   if (rc != 0)
   {
      ocd_drp( &sfi_ocd ) ;
   return(rc);
   }
                                  /* Open the form in READ ONLY mode   */
   *form_ocb = obj_opn (form_oid, OBJ_READ) ;

#else                             /* Open the form in READ ONLY mode   */

   s_init ( & catstr, "* * "                ) ;
   s_copy ( & new_id, catstr,          0, 0 ) ;
   s_cat  ( & new_id, new_id, form_id, 0, 0 );

                                  /* Is it a help screen or a form     */
   if (CHK_MASK(flags, HELP_FORM))
   {

      s_init (&catstr, " HELP");

   }
   else
   {

      s_init (&catstr, " FORM");

   }
   s_cat  (&new_id, new_id, catstr , 0, 0);


   *form_ocb = obj_opn (  (OID *) new_id,  OBJ_READ | OS_FILE) ;

#endif
                                  /* Check to see if open succeded     */

   if ( ! OCB_POINTER_OK( *form_ocb ) )
   {
      rc = -1;
   }
                                  /* Drop the OCD used on the SFI      */
   s_drop ( & catstr   ) ;
   s_drop ( & new_id   ) ;
   os_memd(   form_oid ) ;
   ocd_drp( & sfi_ocd  ) ;

   return(rc);
}
