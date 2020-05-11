/************************************************************************
     <DOC>
     <NAME>    rc_act(OCB *act_ocb, string modifier, hword level,
                                                       byte *params)
     <AUTHOR>  Tanmay S. Kumar
     <DATE>    May 12, 1989.
     <LEVEL>   ACTIVITY
     <PARAM>   name        type      OUTPUT  description
     <PURPOSE> Receive the activity by decrementing the undone count
               and removing it from the undone indexes.
     <OUTPUT>  sint
     <ERRORS>
               ACTIVITY_DONE          6402
               INVALID_OCB            20022
     <LIMITS>  None known.
     </DOC>
************************************************************************/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "exc.h"
#include "isv.h"
#include "activity.h"


sint     rc_act( act_ocb, modifier, level, params )

OCB             *act_ocb;
string          modifier;
hword           level;
byte            *params;

{

 sint     rc               =         0,
          waiting          =         0,
          num_memb         =         0,
          status           =         0;

 OCD      *user_ocd        =       NIL,
          *memb_ocd        =       NIL;

 OCB      *user_ocb        =       NIL,
          *memb_ocb        =       NIL;

 string   result_string    =    S_NULL;

 OID      author_oid                  ,
          memb_oid                    ;

 datetime dt                          ;





if (OCB_POINTER_OK( act_ocb ) )
{

   if (TM_master->usrtype == MASTER_SERVER )
   {
      user_ocb = t_author();

      if ( (level == ACT_LINK_READ) || (level == ACT_READ_READ) )
      {
         obj_find(act_ocb, "ACTIVITY.MEMBERS", &memb_oid);
         memb_ocb = obj_opn(&memb_oid, OBJ_UPDATE);

         if (OCB_POINTER_OK(memb_ocb) )
         {
            ocd_new(&memb_ocd,"MEMBERSHIP.MEMBERS.STATUS");
            oid_key( memb_ocd, &(user_ocb)->oid );

            status = (sint)ISV_DONE;
            obj_rep(memb_ocb, memb_ocd, &status, SEG_UPDATE );
            os_dttm(  & dt  );
            s_init(&(memb_ocd->name),"MEMBERSHIP.MEMBERS.LAST_ACTIVE");
            obj_rep(memb_ocb, memb_ocd, &dt, SEG_UPDATE);
            s_init( &(memb_ocd->name), "MEMBERSHIP.COUNT" );
            num_memb = 0;
            obj_get(memb_ocb, memb_ocd, &num_memb);
            num_memb++;
            obj_rep(memb_ocb, memb_ocd, &num_memb, SEG_UPDATE);
            obj_cls(memb_ocb);
         }
      }
      ocd_new(&user_ocd, "INDEXES.UNDONE_ACT.ID");
      s_copy(&(user_ocd->key), modifier, 0, 0);
      rc = obj_rmv( user_ocb, user_ocd );

      if (rc == 0) /*  remove was sucessfull */
      {
         s_init(&user_ocd->name, "INDEXES.UNDONE_COUNT");
         rc = obj_get( user_ocb, user_ocd, & waiting );
         waiting -- ;
         obj_rep( user_ocb, user_ocd, & waiting, SEG_UPDATE );

         s_init(&user_ocd->name, "INDEXES.ACTIVITIES.OID");
         rc = obj_rep( user_ocb, user_ocd, & (act_ocb->oid),
                                                           SEG_UPDATE);

         rc = obj_find(act_ocb, "%LINKS.AUTHOR", &author_oid);

         if ( !( oid_cmp( (user_ocb->oid), author_oid ) ) )
         {
            waiting = 0;
            s_init( & (user_ocd->name), "INDEXES.ACT_COUNT");
            rc = obj_get( user_ocb, user_ocd, & waiting );
            waiting ++;
            obj_rep( user_ocb, user_ocd, & waiting, SEG_UPDATE );
         }

         ocd_drp( & user_ocd );
      }
      if ( (level != ACT_LINK_RESPONSE) && (level != ACT_LINK_READ) &&
                                          (level != ACT_LINK_SELECTION) )
      {
         t_csend( t_invoker(), rc, result_string );
      }
      obj_cls(user_ocb);
   }
   else
   {
      sint   *undone   =  NIL;

      user_ocb = t_author();
      ocd_new(&user_ocd, "INDEXES.UNDONE_ACT.ID");
      s_copy(&(user_ocd->key), modifier, 0, 0);
      rc = obj_rmv( user_ocb, user_ocd );
      if  (params != NIL)
      {
         undone = (sint *)params;
         if (*undone == ISV_UNDONE)
         {
            rc = 0;
         }
      }
      ocd_drp( & user_ocd );
      if ( (rc == 0) && (level != ACT_LINK_SELECTION) )
      {

         rc = t_remote( TM_master->MS, ACT_OBJ, RECEIVE_ACT, level,
                        act_ocb,  modifier , (void **)(&result_string));
         if (rc == 0)
         {
            except( ACTIVITY_DONE, USER_INFORMATION, S_NULL, S_NULL,
                                                S_NULL, S_NULL, S_NULL );
         }
      }
      obj_cls(user_ocb);
      if ( (rc != INDEX_TO_REMOVE_DOES_NOT_EXIST ) && (rc != 0) )
      {
         except( rc , USER_WARNING, S_NULL, S_NULL, S_NULL, S_NULL,
                                                                S_NULL );
      }

   }
}
else
{
   rc = INVALID_OCB;
}

   return(rc);

}
