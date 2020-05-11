/*+-------------------------------------------------------------------+
  |  <Doc>                                                            |
  |  <NAME>    vw_cr                                                  |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    February 14, 1989                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *      INPUT  Conference to view.      |
  |  <PARAM>   modifier    string     INPUT  Conference to view.      |
  |  <PARAM>   level       hword      INPUT  Type of view to do.      |
  |  <PARAM>   params      void *    OUTPUT  What to return.          |
  |  <PURPOSE> View   Conference Root                                 |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "isv.h"
#include "form.h"
#include "vars.h"
#include "obj_list.h"
#include "obj_cr.h"
#include "obj_cc.h"


sint vw_cr( conf_ocb, modifier, level, params )

OCB            *conf_ocb;
string          modifier;
hword           level;
void           *params;

{

sint        rc            =       NO_ERROR,
            cc_count      =            0,
            state         =            0;
hword       flags         =            0,
            command       = SEND_SEGMENT;
string      packet        =       S_NULL,
            result_packet =       S_NULL,
            tmp_str       =       S_NULL,
            NAMES         =       S_NULL,
            form_id       =       S_NULL,
            cr_id         =       S_NULL;
OID         tmp_oid                     ,
          * conf_oid      =     &tmp_oid,
            tmpxoid                     ,
          * conf_list_oid =     &tmpxoid;
OCB       * user_ocb      =          NIL,
          * conf_list_ocb =          NIL,
          * comment_ocb   =          NIL;
OCD       * user_oid_ocd  =          NIL,
          * user_count_ocd=          NIL,
          * component     =          NIL;
boolean     CONF_OCB_OPENED    =   FALSE,
            MEMBER_OF_ANY_CONF =   FALSE,
            USER_GAVE_US_LABEL =   FALSE,
            presented_form     =   FALSE,
            done               =   FALSE;






if( level == ISV_NEW )
{

   level = (SCAN_ALL_CONFS | SCAN_ALL_COMMENT) ;

}
if( level & VIEWING_COMMENT )
{

                           /* if coming from VW_CC, params is a cc_ocb */
   comment_ocb = (OCB *) params;

}
ocd_new( & user_oid_ocd,   "MEMBERSHIPS.CONFERENCES" );
ocd_new( & user_count_ocd, "INDEXES.CC_COUNT"        );
os_memn(   conf_oid,       sizeof( OID )             );
os_memn(   conf_list_oid,  sizeof( OID )             );

user_ocb = t_author();

while( NOT done )
{

   done = TRUE;               /* Assume there are no errors        */


   rc = cfr_update_conference_view();

   if( ( ! OCB_POINTER_OK( conf_ocb ) )
    && (   conf_ocb->type != CONF_OBJ ) )
   {
                                  /* - - - - - - - - - - - - - - - - - */
                                  /* In future we might want to allow  */
                                  /* list_ocb as 1st parameter also. so*/
                                  /* we view all confs listed in that  */
                                  /* list object.                      */
                                  /*                                   */
                                  /* If modifier is provided assume it */
                                  /* is a list of CR's to view.  If not*/
                                  /* provided or error on them, present*/
                                  /* a form to request from the user.  */
                                  /* - - - - - - - - - - - - - - - - - */

      if( ( s_len( modifier ) < 1 )
      ||  (rc == CONF_ID_NOT_IN_CRI) || (rc == CONF_ACCESS_NOT_ALLOWED) )
      {

         rc = cfr_gtid( rc, & cr_id, & level );
         presented_form = TRUE;

      }
      else
      {

         s_copy( & cr_id, modifier, 0, 0 );

      }
      if( rc != NO_ERROR )
      {

         done = TRUE;
         continue;

      }
      if( ! (level & SCAN_ALL_CONFS) )
      {

                                       /* - - - - - - - - - - - - - - -*/
                                       /* If conf doesn't exist in usrs*/
                                       /* ocb list, then lets see if he*/
                                       /* is joining public conf for   */
                                       /* the 1st time or user gave    */
                                       /* LABEL rather then ID. We     */
                                       /* don't keep index of conf     */
                                       /* labels in user ocb, we've to */
                                       /* go to MS to check in CRI     */
                                       /* - - - - - - - - - - - - - - -*/

         s_catx( & tmp_str,  cr_id, 0, 0 );
         s_catx( & tmp_str,  cr_id, 0, 0 );

         rc = ex_cr(   NIL,  tmp_str,
                       CHK_CONF_LABEL | CHK_CONF_ID,
                       & conf_oid                        );

         USER_GAVE_US_LABEL = TRUE;

         s_drop( & tmp_str );

         if(      (rc == CONF_ID_ALREADY_IN_CRI)
              ||  (rc == CONF_LABEL_ALREADY_IN_CRI)  )
         {

            rc = NO_ERROR;

         }
      }
      if(  ( rc == NO_ERROR )  &&  ( ! OID_IS_NULL( conf_oid ) )  )
      {

         conf_ocb = obj_opn( conf_oid, OBJ_UPDATE | OBJ_REMOTE );

         if( OCB_POINTER_OK( conf_ocb ) )
         {

            CONF_OCB_OPENED = TRUE;

            if( USER_GAVE_US_LABEL )
            {

               obj_find( conf_ocb, "CONFERENCE.ID", & cr_id );

            }
         }
      }
      else
      {

         except( rc, USER_ERROR, cr_id, S_NUL, S_NUL, S_NUL, S_NUL );
         if( presented_form )
         {

            done = FALSE;

         }
         else
         {

            done =  TRUE;

         }
      }
   }
   if( rc == NO_ERROR )
   {

      if( level & SCAN_ALL_CONFS )
      {

         MEMBER_OF_ANY_CONF = FALSE;
         done               =  TRUE;

         rc = obj_get( user_ocb, user_oid_ocd, conf_list_oid );
         conf_list_ocb = obj_opn( conf_list_oid,
                                               OBJ_UPDATE | OBJ_REMOTE );

         rc = obj_get( user_ocb, user_count_ocd, & cc_count );

         if( cc_count == 0 )
         {

            rc = NO_COMMENTS_TO_LIST;
            except( rc, USER_WARNING, S_NIL, S_NIL, S_NIL, S_NIL, S_NIL );

         }

         MEMBER_OF_ANY_CONF = TRUE;
         rc = gt_list(    conf_list_ocb,   modifier,
                          level,           (byte *) cc_count );
         if( rc == USER_REQUESTED_QUIT )
         {

            rc = NO_ERROR;
            break;

         }
         else if ( rc == USER_REQUESTED_ABANDON )
         {

            break;

         }
         rc = obj_get( user_ocb, user_count_ocd, & cc_count );
         rc = obj_cls( conf_list_ocb );
         level &= ~SCAN_ALL_CONFS;

         if( MEMBER_OF_ANY_CONF == FALSE )
         {

            done = TRUE;
            rc   = CONF_NOT_MEMBER_OF_ANY;

         }
      }
      else
      {

         obj_get(    user_ocb, user_count_ocd,   & cc_count );
         rc   = gt_cr( conf_ocb, modifier, level,  cc_count );
         done = TRUE;

      }
   }
   if( CONF_OCB_OPENED )
   {

      obj_set_flag( conf_ocb, OBJ_DELETE );
      obj_cls(      conf_ocb             );
      conf_ocb    = NIL;

   }

}                                /*         while not_done             */

ocd_drp( & user_count_ocd );
ocd_drp( & user_oid_ocd   );
ocd_drp( & component      );
obj_cls(   user_ocb       );

s_init ( & NAMES, "NAMES" );     /* Set in CFR_GTID                    */
var_clr(   NAMES          );
s_drop ( & NAMES          );

s_init ( & form_id, "1140"   );  /* Opened in CC_GTID                  */
f_close(   form_id, NO_FLAGS );
s_drop ( & form_id           );

s_drop(  & cr_id          );
s_drop(  & result_packet  );

   return(rc);

}
