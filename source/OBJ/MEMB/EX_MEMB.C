/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ex_memb                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    March 27, 1989                                         |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PARAM>   name        type      OUTPUT  description              |
  |  <PURPOSE> Check for existance of a membership list.              |
  |  <OUTPUT>  sint                                                   |
  |  <LIMITS>  None known.                                            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_prim.h"




sint    ex_memb( ocb, memb_oid, level, result_priv )

OCB             *ocb;
OID             *memb_oid;
hword            level;
PRIVS           *result_priv;

{

sint     rc             = NO_ERR;

byte     *ocd_name      = NULL;

OCB      *memb_ocb      = NULL;

OCD      *memb_ocd      = NULL;

OID       tmp_oid;

boolean   MEMB_OCB_OPENED = FALSE;



if( OCB_POINTER_OK(ocb) )
{

   switch( ocb->type )
   {

   case CONF_OBJ     :
      ocd_name = "CONFERENCE.MEMBERS";
      break;

   case GROUP_OBJ    :
      ocd_name = "GROUP.MEMBERS";
      break;

   case ACTIVITY_OBJ :
      ocd_name = "ACTIVITY.MEMBERS";
      break;

   case MEMBERSHIP_OBJ :
      memb_ocb = ocb;
      break;

   default :
      rc = IMPROPER_OBJECT_TYPE;

   }  /* Switch */


   if( rc == NO_ERR )
   {

      if( ocb->type != MEMBERSHIP_OBJ )
      {                           /* Then try to open membership ocb   */

         if( obj_find( ocb, ocd_name, &tmp_oid) == NO_ERR )
         {

            memb_ocb = obj_opn( &tmp_oid, OBJ_UPDATE | OBJ_REMOTE );
            MEMB_OCB_OPENED = TRUE;

         }
         else
         {

            rc = MEMBERSHIP_DOES_NOT_EXIST;

         }

      }  /* if object is not membership */

      if( rc == NO_ERR )
      {

         ocd_new( &memb_ocd, "MEMBERSHIP.MEMBERS.ROLES" );
         oid_key( memb_ocd, memb_oid );
         memb_ocd->bias = 1;

         if( obj_get(memb_ocb, memb_ocd, result_priv) != NO_ERR )
         {

            rc = MEMBER_DOES_NOT_EXIST;

         }
         ocd_drp( &memb_ocd );


      }  /* If no problem getting membership object */

      if( MEMB_OCB_OPENED )
      {

         obj_cls( memb_ocb );

      }

   }  /* If proper object */

}
else
{

   rc = INVALID_OCB;

}


   return(rc);

}
