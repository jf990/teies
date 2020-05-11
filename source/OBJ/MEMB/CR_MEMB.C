/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cr_memb( )                                             |
  |  <AUTHOR>  Gurinder Singh Johar                                   |
  |  <DATE>    Jan 19, 1989                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT  Owner OCB pointer        |
  |  <PARAM>   modifier    string     INPUT  Name of the owner        |
  |  <PARAM>   level       hword      INPUT  Flags Defined at bottom  |
  |  <PARAM>   memb_ocb    OCB **     INOUT  Return the object we     |
  |                                          allocated in this routine|
  |  <PURPOSE> Create Memebership object.                             |
  |  <OUTPUT>  returns  2  if QUIT requested, and "cnotrol" == 9      |
  |  <CALLS>   s_init(), t_form(), isv_get(), isv_match(), s_comp(),  |
  |            f_setfld(), f_rstfld(), ocd_new(), obj_rep(), obj_enc()|
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_prim.h"

sint    cr_memb( owner_ocb, modifier, level, membership_ocb )

OCB            * owner_ocb;
string           modifier;
hword            level;
OCB           ** membership_ocb;

{

OID      owner_oid,
        *oid_ptr          = & owner_oid;

OCB     *memb_ocb         = NULL;

boolean  OWNER_OCB_OPENED = FALSE;

byte    *ocd_name         = NULL;

sint     rc               = NO_ERR,
         bad_itm_num      = 0;

string   bad_itm_str      = S_NULL;

hword    flags            = MEMB_OCB_ALREADY_CHECKED,
         junk             = 0;






if( ! OCB_POINTER_OK( owner_ocb ) )
{                               /* If passed owner OCB was no GOOD     */


   if( s_len( modifier ) > 0 )  /* Then check if modifer has name of   */
   {                            /* the owner. If so, find OCB via OID  */

      if( (rc = obj_item( modifier, OBJ_NEED_ONE_OID, &oid_ptr, NULL,
                         &bad_itm_str, &bad_itm_num, junk )) == NO_ERR )
      {                         /* If NO problems getting the OID from */
                                /* name of the item                    */

         owner_ocb = obj_opn( & owner_oid, OBJ_UPDATE );
         OWNER_OCB_OPENED = TRUE;
                                  /* Owner already exists, link object */
         level |= (MEMB_DOING_CREATION | MEMB_DOING_LINKING) ;

         if( OCB_POINTER_OK( owner_ocb ) == FALSE )
         {

            rc = INVALID_OCB;

         }

      }
      else
      {                           /* We could not determine owner even */
                                  /* from modifier string              */
                                  /* May be we should put up a form and*/
                                  /* ask for the item???               */
         rc = INVALID_OCB;

      }

   }
   else                           /* We could not determine owner.     */
   {

      rc = INVALID_OCB;

   }

}

if( rc == NO_ERR )
{
                                  /* If we found the owner OCB then we */
                                  /* make sure it is of correct type.  */
                                  /* If LEVEL tells us to use object   */
                                  /* already allocated for membership, */
                                  /* then we simply use the object else*/
                                  /* we allocate new membership object */
                                  /* Then call md_memb() with OCB_ALRE */
                                  /* ADY_CHECKED flag and if level says*/
                                  /* use existing membership object,   */
                                  /* then also OR OCB_ALREADY_LINKED   */

   switch( owner_ocb->type )
   {

   case CONFER_OBJ   :

      ocd_name = "CONFERENCE.MEMBERS";
      break;

   case GROUP_OBJ    :

      ocd_name = "GROUP.MEMBERS";
      break;

   case ACTIVITY_OBJ :

      ocd_name = "ACTIVITY.MEMBERS";
      break;

   default           :

      rc = IMPROPER_OBJECT_TYPE;
      break;

   }

   if( rc == NO_ERR )
   {

      rc = obj_find( owner_ocb, ocd_name, & owner_oid );

      if( OID_IS_NULL( & owner_oid ) )
      {

         rc = NO_DATA_FOUND;

      }
      if( rc == NO_ERR )
      {

         memb_ocb = obj_opn( & owner_oid, OBJ_UPDATE );
         if( OCB_POINTER_OK( memb_ocb ) )
         {

            if( (level & MEMB_USE_EXISTING_OBJECT) == 0 )
            {

               rc = MEMBERSHIP_ALREADY_EXISTS;

            }
         }
         else
         {

            rc = INVALID_OCB;

         }
      }
      else if((rc == NO_DATA_FOUND) && (level & MEMB_USE_EXISTING_OBJECT)==0 )
      {

         memb_ocb = obj_all( MEMBERSHIP_OBJ, LOCAL_OBJECT, NULL);
         obj_update( memb_ocb, "%LINKS.OWNER", &(owner_ocb->oid) );
         rc = NO_ERR;

      }

      if( rc == NO_ERR )
      {

         if( OCB_POINTER_OK( memb_ocb ) )
         {

            rc = md_memb( memb_ocb, S_NULL, flags | level, & owner_ocb );
            if( membership_ocb != NIL )
            {

               * membership_ocb = memb_ocb;

            }
         }
         else
         {

            rc = CANNOT_CREATE_OBJECT;

         }

      }  /* If membership was already not there */

   }  /* If owner object type is correct */

}  /* If there were no problems getting the owner OCB */


if( rc == NO_ERR )
{

                                 /* set up the link between owner and  */
                                 /* membership object                  */

    obj_update( owner_ocb, ocd_name, &(memb_ocb->oid) );

                    /* Don't close the object here...if creating owner */
                    /* It may as well choose to abort creation         */
                    /* process and delete it!!                         */

   if( ! (level & MEMB_DOING_CREATION) )
   {

      obj_cls( memb_ocb );

   }
}
else if( rc == OBJECT_NOT_LINKED )
{

   rc = NO_ERR;

}

if( OWNER_OCB_OPENED )
{

   obj_cls( owner_ocb );

}
   return(rc);

}
