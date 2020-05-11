/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    lk_grup( )                                             |
  |  <AUTHOR>  Rana Ajaz-Ur-Rehman,  Tanmay S. Kumar                  |
  |  <DATE>    September 6, 1988.    August 23, 1989                  |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   group_ocb   OCB *     INPUT   Group Object Control     |
  |                                          Block to take components |
  |                                          from                     |
  |                                                                   |
  |            modifier    string    INPUT   Not used.                |
  |                                                                   |
  |            level       hword     INPUT   Indicates  what to be do |
  |                                                                   |
  |            params      byte *    IN/OUT  used for returning values|
  |                                                                   |
  |  <PURPOSE> Link a Group to the system group index and associated  |
  |            objects.                                               |
  |            Link to System Group Index ( SGI )                     |
  |                 Primary Keys are                                  |
  |                 (i) - Group ID                                    |
  |                 (ii)- Group Label                                 |
  |                                                                   |
  |                                                       Not         |
  |            Link to owner user if private Group     )  implemented |
  |            Link to Membership index                )  yet         |
  |  <OUTPUT>  Sint.                                                  |
  |  <CALLS>   t_sysndx(), obj_find(), ocd_new(), s_copy(), obj_rep() |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "vars.h"
#include "exc.h"
#include "form.h"
#include "isv.h"
#include "obj_group.h"
#include "obj_memb.h"
#include "obj_name.h"


sint     lk_grup( group_ocb, modifier, level, params )

OCB      *group_ocb;
string    modifier;
hword     level;
byte     *params;

{

   OCB      *sgi_ocb    =     NULL,
            *memb_ocb   =     NULL,
            *user_ocb   =     NULL;

   OID      tmp_oid               ;

   byte    **tmp_ptr    =     NULL;

   OCD      *sgi_idocd  =     NULL,
            *sgi_labocd =     NULL,
            *tmp_ocd    =     NULL,
            *group_ocd  =     NULL;

   sint      rc         =   NO_ERR,
             last_id    =        0;

   string    group_id    =  S_NULL,
             name_str   =  S_NULL,
             group_label =  S_NULL;



if( TM_master->usrtype == MASTER_SERVER )
{

   sgi_ocb = t_sysndx( SGI_OBJ         );    /* Get SGI OCB to look if */

   ocd_new( &tmp_ocd, "GROUP.ID" );
   obj_get( group_ocb, tmp_ocd, &group_id );
   ocd_new( &sgi_idocd, "SGI.ID.OID"       );
   s_copy( &(sgi_idocd->key), group_id, 0, 0      );
   if( obj_get( sgi_ocb, sgi_idocd, &tmp_oid) == NO_DATA_FOUND)
   {                        /* Now lets chk and see label is availble? */

      s_init( &(tmp_ocd->name), "GROUP.LABEL" );
      obj_get( group_ocb, tmp_ocd, &group_label   );
      ocd_new( &sgi_labocd, "SGI.LABEL.OID"             );
      s_copy( &(sgi_labocd->key), group_label, 0, 0            );

      if( obj_get( sgi_ocb, sgi_labocd, &tmp_oid ) == NO_DATA_FOUND)
      {


         rc = obj_rep(sgi_ocb,sgi_idocd ,&(group_ocb->oid),SEG_UPDATE);
         rc = obj_rep(sgi_ocb,sgi_labocd,&(group_ocb->oid),SEG_UPDATE);

         s_init ( &(tmp_ocd->name), "%LINKS.OWNER" );
         obj_get( group_ocb, tmp_ocd, &tmp_oid );
         user_ocb = obj_opn( &tmp_oid, OBJ_UPDATE );

         rc = gt_name(user_ocb, S_NULL, FULL_TEIES_NAME, &name_str);
         s_init ( &(tmp_ocd->name), "GROUP.OWNER" );
         obj_rep( group_ocb, tmp_ocd, name_str, SEG_UPDATE);

         s_init ( &(tmp_ocd->name), "MEMBERSHIPS.LAST_GROUP_ID" );
         obj_get( user_ocb, tmp_ocd, &last_id );
         last_id++;
         obj_rep( user_ocb, tmp_ocd, &last_id, SEG_UPDATE );

         last_id = 0;
         s_init ( &(tmp_ocd->name), "MEMBERSHIPS.OWNER_GROUP_C");
         obj_get( user_ocb, tmp_ocd, &last_id );
         last_id++;
         obj_rep( user_ocb, tmp_ocd, &last_id, SEG_UPDATE );

         s_init ( &(tmp_ocd->name), "MEMBERSHIPS.GROUP_OWNER.OID" );
         s_copy( &(tmp_ocd->key), group_id, 0, 0 );
         obj_rep( user_ocb, tmp_ocd, &(group_ocb->oid), SEG_UPDATE );
         obj_cls(user_ocb);

      }
      else
      {

         rc = GROUP_LABEL_ALREADY_IN_SGI;

      }

   }
   else
   {

      rc = GROUP_ID_ALREADY_IN_SGI;

   }

   if (OCB_POINTER_OK (sgi_ocb) )
   {
      obj_cls(sgi_ocb);
   }

                                 /* If there are no problems then link */
   if( rc == NO_ERR )            /* membership object info also        */
   {

      s_init( &(tmp_ocd->name), "GROUP.MEMBERS" );
      if( (rc = obj_get(group_ocb, tmp_ocd, &tmp_oid)) == 0 )
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
            obj_upd(group_ocb, "GROUP.MEMBERS",&(memb_ocb->oid));
            obj_cls( memb_ocb );

         }

         t_m_obj( t_invoker(), group_ocb, S_NULL, FALSE, 0 );
         rc = DO_NOT_CONFIRM_TRANSACTION;

      }

   }

   ocd_drp( &tmp_ocd    );
   ocd_drp( &sgi_idocd  );
   ocd_drp( &sgi_labocd );
   s_drop(  &group_id    );
   s_drop(  &group_label );

   obj_cls( group_ocb );       /* caller left it open for us         */

}
else
{

   obj_find( group_ocb, "GROUP.MEMBERS", &tmp_oid );

   rc = t_spawn( group_ocb, modifier, (sint) level, params );

   if( (rc == NO_ERR) && (tmp_oid.one != 0) )
   {                                   /* If there is MEMB ocb, then   */
                                       /* accept it back               */
      rc =t_waitc(TM_master->MS, tmp_ptr); /* Now wait for updated MEMB*/

   }



}

   return(rc);

}

