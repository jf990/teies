/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    lk_cr( )                                               |
  |  <AUTHOR>  Gurinder S. Johar                                      |
  |  <DATE>    Feb 02, 1989                                           |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   conf_ocb    OCB *     INPUT   Conference Object Control|
  |                                          Block to take components |
  |                                          from                     |
  |  <Param>   modifier    string    INPUT   Not used.                |
  |  <param>   level       hword     INPUT   Not sure about this.     |
  |  <param>   params      byte *    INPUT   Not used.                |
  |                                                                   |
  |  <PURPOSE> Link a Conference to different System Indexes          |
  |                                                                   |
  |            Link to Conference Root Index                          |
  |                 Primary Keys are                                  |
  |                 (i) - Conference ID                               |
  |                 (ii)- Conference Label                            |
  |                                                                   |
  |            Link to Group                           )  Not         |
  |            Link to owner user if private conference)  implemented |
  |            Link to Membership index                )  yet         |
  |                                                                   |
  |  <OUTPUT>  returns  7  so that CFR_CFRM() call be called          |
  |  <CALLS>   t_sysndx(), obj_find(), ocd_new(), s_copy(), obj_rep() |
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


sint     lk_cr( conf_ocb, modifier, level, params )

OCB      *conf_ocb;
string    modifier;
hword     level;
string   *params;

{

   OCB      *cri_ocb    =     NULL,
            *memb_ocb   =     NULL,
            *user_ocb   =     NULL;

   OCD      *cri_idocd  =     NULL,
            *cri_labocd =     NULL,
            *tmp_ocd    =     NULL,
            *conf_ocd   =     NULL;

   OID       tmp_oid;

   byte    **tmp_ptr    =  NULL;


   sint      rc         =  NO_ERR,
             last_id    =  0;

   string    conf_id    =  S_NULL,
             name_str   =  S_NULL,
             work_str   =  S_NULL,
             key_words  =  S_NULL,
             conf_label =  S_NULL;



if( TM_master->usrtype == MASTER_SERVER )
{

   cri_ocb = t_sysndx( CRI_OBJ         );    /* Get CRI OCB to look if */

   ocd_new( &tmp_ocd, "CONFERENCE.ID" );
   obj_get( conf_ocb, tmp_ocd, & conf_id );
   s_upper( conf_id,                         0, 0 );
   ocd_new( & cri_idocd,      "CRI.ID.CONF_ROOT"  );
   s_copy(  &(cri_idocd->key), conf_id,      0, 0 );

   if( obj_get( cri_ocb, cri_idocd, & tmp_oid ) == NO_DATA_FOUND )
   {                        /* Now lets chk and see label is availble? */

      s_init(  &(tmp_ocd->name), "CONFERENCE.LABEL"      );
      obj_get(   conf_ocb,       tmp_ocd, & conf_label   );
      s_upper(   conf_label,                        0, 0 );
      ocd_new( & cri_labocd,     "CRI.LABEL.CONF_ROOT"   );
      s_copy(  &(cri_labocd->key), conf_label,      0, 0 );

      if( obj_get( cri_ocb, cri_labocd, & tmp_oid ) == NO_DATA_FOUND )
      {


         rc = obj_rep( cri_ocb, cri_idocd , &(conf_ocb->oid),SEG_UPDATE);
         rc = obj_rep( cri_ocb, cri_labocd, &(conf_ocb->oid),SEG_UPDATE);

         s_init ( &(tmp_ocd->name), "%LINKS.OWNER" );
         obj_get( conf_ocb, tmp_ocd, &tmp_oid );
         user_ocb = obj_opn( &tmp_oid, OBJ_UPDATE );

         rc = gt_name(user_ocb, S_NULL, FULL_TEIES_NAME, &name_str);
         s_init ( &(tmp_ocd->name), "CONFERENCE.OWNER" );
         obj_rep( conf_ocb, tmp_ocd, name_str, SEG_UPDATE);

         s_init ( &(tmp_ocd->name), "MEMBERSHIPS.LAST_CONF_ID" );
         obj_get( user_ocb, tmp_ocd, &last_id );
         last_id++;
         obj_rep( user_ocb, tmp_ocd, &last_id, SEG_UPDATE );

         last_id = 0;
         s_init ( &(tmp_ocd->name), "MEMBERSHIPS.OWNER_CONF_C" );
         obj_get( user_ocb, tmp_ocd, &last_id );
         last_id++;
         obj_rep( user_ocb, tmp_ocd, &last_id, SEG_UPDATE );

         rc = add_to_user_list( user_ocb, "MEMBERSHIPS.CONF_OWNER",
                                conf_id, & (conf_ocb->oid), 0, S_NULL );

         s_init ( & (tmp_ocd->name), "CONFERENCE.KEYS" );
         obj_get(       conf_ocb, tmp_ocd, & key_words );
         rc     =      t_ckkeys( & work_str, key_words );
         if( rc == 0 )
         {

            obj_update( conf_ocb, "CONFERENCE.KEYS", work_str );

         }
         s_drop( & key_words );
         s_drop( & work_str  );

      }
      else
      {

         rc = CONF_LABEL_ALREADY_IN_CRI;

      }
   }
   else
   {

      rc = CONF_ID_ALREADY_IN_CRI;

   }

                                 /* If there are no problems then link */
   if( rc == NO_ERR )            /* membership object info also        */
   {

      s_init( &(tmp_ocd->name), "CONFERENCE.MEMBERS" );
      if( (rc = obj_get(conf_ocb, tmp_ocd, &tmp_oid)) == 0 )
      {

         if( tmp_oid.one != 0 )
         {

            memb_ocb = obj_opn( &tmp_oid, OBJ_UPDATE );
            level |= MEMB_DOING_LINKING;

            rc = lk_memb( memb_ocb, S_NULL, level, S_NULL );
            obj_cls( memb_ocb );

         }
         else
         {

            memb_ocb = obj_all( MEMBERSHIP_OBJ, LOCAL_OBJECT, NULL );
            obj_upd(conf_ocb, "CONFERENCE.MEMBERS",&(memb_ocb->oid));
            obj_cls( memb_ocb );

         }

         t_m_obj( t_invoker(), conf_ocb, S_NULL, FALSE, 0 );
         rc = DO_NOT_CONFIRM_TRANSACTION;

      }
   }
   ocd_drp( & tmp_ocd    );
   ocd_drp( & cri_idocd  );
   ocd_drp( & cri_labocd );
   s_drop(  & conf_id    );
   s_drop(  & conf_label );

}
else
{

   obj_find( conf_ocb, "CONFERENCE.MEMBERS", & tmp_oid );

   rc = t_spawn( conf_ocb, modifier, (sint) level, params );

   if( (rc == NO_ERROR) && (! OID_IS_NULL( & tmp_oid ) ) )
   {                                   /* If there is MEMB ocb, then   */
                                       /* accept it back               */
      rc = t_waitc( TM_master->MS, tmp_ptr );

   }
}

   return(rc);

}
