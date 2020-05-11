/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    lk_memb                                                |
  |  <AUTHOR>  Sal Johar                                              |
  |  <DATE>    August 13, 1987                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   memb_ocb    struct OCB *   INPUT  Object of the memb-  |
  |                                              ership type.         |
  |                                                                   |
  |  <PARAM>   modifier    type      OUTPUT  description              |
  |  <PARAM>   level       type      OUTPUT  description              |
  |  <PARAM>   result      type      OUTPUT  description              |
  |                                                                   |
  |  <PURPOSE> Link an object to a membership object.  A membership   |
  |            object is attached to another object.  Link means      |
  |            either to attach the membership object onto the owner  |
  |            object, or to attach an object that will belong to the |
  |            membership.                                            |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_prim.h"
#include "tm.h"
#include "isv.h"



sint     lk_memb( memb_ocb, modifier, level, result_str )

OCB             * memb_ocb;
string            modifier;
hword             level;
string          * result_str;

{

sint    rc = NO_ERROR;




if( ( ! OCB_POINTER_OK( memb_ocb )     )
||  ( memb_ocb->type != MEMBERSHIP_OBJ ) )
{

   return(INVALID_OCB);

}

if( TM_master->usrtype != MASTER_SERVER )
{

   rc = t_spawn( memb_ocb, modifier, (sint) level, result_str );

}
else
{


string   owner_id          = S_NULL,
         conf_lab          = S_NULL,
         members_to_add    = S_NULL,
         members_to_modify = S_NULL,
         members_to_remove = S_NULL,
         add_err           = S_NULL,
         mod_err           = S_NULL,
         rem_err           = S_NULL,
         add_ntf           = S_NULL,
         mod_ntf           = S_NULL,
         rem_ntf           = S_NULL,
         roles_ocd         = S_NULL,
         root_ocd          = S_NULL,
         date_ocd          = S_NULL,
         active_ocd        = S_NULL,
         tmp_mocd          = S_NULL,
         tmp_uocd          = S_NULL,
         memb_roles        = S_NULL,
         usr_count         = S_NULL,
         usr_ndx           = S_NULL,
         prefix            = S_NULL,
         tmpstr            = S_NULL,
         tmpstr2           = S_NULL;
OCD     * memb_ocd         =    NIL,
        * user_ocd         =    NIL,
        * list_ocd         =    NIL;
OCB     * user_ocb         =    NIL,
        * owner_ocb        =    NIL,
        * add_memb_list    =    NIL,
        * modify_memb_list =    NIL,
        * remove_memb_list =    NIL;
OID     * oidkey           =    NIL,
          owner_oid                ,
        * oid_ptr          = & owner_oid;
datetime  cur_dt                        ,
          last_active                   ;
sint      arc              =    NO_ERROR,
          mrc              =    NO_ERROR,
          rrc              =    NO_ERROR,
          count            =           0,
          am_count         =           0,
          mm_count         =           0,
          rm_count         =           0,
          memb_root        =           0,
          oid_len          =           0,
          i                =           0;
hword     owner_type       =           0;
byte    * user_index       =         NIL;
OCD     * tmp_ocd          =         NIL;
PRIVS     tmppriv                       ;




   obj_find(  memb_ocb, "TMP.ADDMEMB", & members_to_add    );
   obj_find(  memb_ocb, "TMP.MODMEMB", & members_to_modify );
   obj_find(  memb_ocb, "TMP.REMMEMB", & members_to_remove );
   ocd_new( & list_ocd, "LIST.SIZE" );

   if( level  &  MEMB_DOING_CREATION )
                                           /* Are we trying to link??  */
   {                                       /* If no, then check if user*/
                                           /* names are valid otherwise*/
                                           /* assume names were validat*/
                                           /* when this routine was    */
                                           /* called while cr_memb()   */

      if( s_len( members_to_add ) > 0 )
      {

         if( level  &  MEMB_USE_EXISTING_LISTS )
         {

            obj_find(     memb_ocb, "TMP.ADDOID",       oid_ptr      );
            add_memb_list = obj_opn( oid_ptr,           OBJ_UPDATE   );
            rc = obj_get( add_memb_list, list_ocd,    & am_count     );

         }
         else
         {
                                 /* - - - - - - - - - - - - - - - - -  */
                                 /* During the creation process we     */
                                 /* store all user OIDs in Lists in    */
                                 /* order to have them at our disposal */
                                 /* when ready to do the real link, as */
                                 /* well as have them already validated*/
                                 /* These List objects must be deleted */
                                 /* after the linking process is com-  */
                                 /* plete or the process is abandon.   */
                                 /* - - - - - - - - - - - - - - - - -  */

            rc = cr_list( NULL, S_NULL, OBJ_LIST_INIT, & add_memb_list );
            obj_update( memb_ocb, "TMP.ADDOID", & (add_memb_list->oid) );

         }
         rc = ex_user( add_memb_list, members_to_add,
                           STRING_TO_OID | NO_NEED_TO_XMIT, result_str );
         obj_get( add_memb_list, list_ocd, & am_count );


      }


      if( (rc == NO_ERROR) && (s_len( members_to_modify ) > 0) )
      {

         if( level &  MEMB_USE_EXISTING_LISTS )
         {

            obj_find(     memb_ocb, "TMP.MODOID",        oid_ptr );
            modify_memb_list = obj_opn( oid_ptr,      OBJ_UPDATE );
            rc = obj_get( modify_memb_list, list_ocd, & mm_count );

         }
         else
         {

            rc = cr_list( NIL, S_NULL, OBJ_LIST_INIT, & modify_memb_list );
            obj_update( memb_ocb, "TMP.MODOID", & (modify_memb_list->oid) );

         }

         rc = ex_user( modify_memb_list, members_to_modify,
                           STRING_TO_OID | NO_NEED_TO_XMIT, result_str );
         obj_get( modify_memb_list, list_ocd, & mm_count );


      }


      if( (rc == NO_ERROR) && (s_len( members_to_remove ) > 0) )
      {

         if( level &  MEMB_USE_EXISTING_LISTS )
         {

            obj_find(   memb_ocb,  "TMP.REMOID",               oid_ptr );
            remove_memb_list = obj_opn(   oid_ptr,          OBJ_UPDATE );
            rc       = obj_get( remove_memb_list, list_ocd, & rm_count );

         }
         else
         {

            rc = cr_list( NIL, S_NULL, OBJ_LIST_INIT, & remove_memb_list );
            obj_update( memb_ocb, "TMP.REMOID", & (remove_memb_list->oid) );

         }

         rc = ex_user( remove_memb_list, members_to_remove,
                           STRING_TO_OID | NO_NEED_TO_XMIT, result_str );
         obj_get( remove_memb_list, list_ocd, & rm_count );


      }
   }


   if(      (level &  MEMB_DOING_LINKING  )
       &&   (rc    == 0                   ) )
   {
                                 /* = = = = = = = = = = = = = = = = = =*/
                                 /* Names have already been validated  */
                                 /* so lets get respective OIDs for    */
                                 /* List OCBs                          */
                                 /* If in this call we did creation    */
                                 /* also, then we do NOT need to open  */
                                 /* list objects                       */
                                 /* = = = = = = = = = = = = = = = = = =*/

      if( ! (level & MEMB_DOING_CREATION) )
      {

         obj_find( memb_ocb, "TMP.ADDOID", oid_ptr    );
         if( !  OID_IS_NULL( oid_ptr ) )
         {

            add_memb_list = obj_opn( oid_ptr, OBJ_UPDATE );

            if( OCB_POINTER_OK(add_memb_list) )
            {

               rc = obj_get( add_memb_list, list_ocd, & am_count );

            }
         }
         obj_find( memb_ocb, "TMP.MODOID", oid_ptr );
         if( !  OID_IS_NULL( oid_ptr ) )
         {

            modify_memb_list = obj_opn( oid_ptr, OBJ_UPDATE );

            if( OCB_POINTER_OK(modify_memb_list) )
            {

               rc = obj_get( modify_memb_list, list_ocd, & mm_count );

            }
         }
         obj_find( memb_ocb, "TMP.REMOID", oid_ptr );
         if( !  OID_IS_NULL( oid_ptr ) )
         {

            remove_memb_list = obj_opn( oid_ptr, OBJ_UPDATE );

            if( OCB_POINTER_OK(remove_memb_list) )
            {

               rc = obj_get( remove_memb_list, list_ocd, & rm_count );

            }
         }
      }

      rc = NO_ERROR;

      obj_find(  memb_ocb,      "TMP.ROOT", & memb_root );

      s_alloc( & memb_roles,    sizeof( PRIVS )         );
      s_smp_ul(  memb_roles ) = sizeof( PRIVS )          ;
      obj_find(  memb_ocb,      "TMP.ROLES",   s_buffer(memb_roles) );

      obj_find(  memb_ocb,      "%LINKS.OWNER", & owner_oid    );

      obj_find(  memb_ocb,      "TMP.OWNID"  ,  & owner_id     );
      obj_find(  memb_ocb,      "TMP.OWNTYPE",  & owner_type   );



      switch( owner_type )
      {

         case CONFERENCE_OBJ :

            s_init( &  add_ntf,    "You have been ADDED to Conference ");
            s_init( &  tmpstr ,    " in Conference "                   );
            s_init( &  rem_ntf, "You have been REMOVED from Conference ");
            user_index    =        "MEMBERSHIPS.CONFERENCES"            ;
            break;

         case GROUP_OBJ :

            s_init( &  add_ntf,    "You have been ADDED to Group "     );
            s_init( &  tmpstr ,    " in Group "                        );
            s_init( &  rem_ntf,    "You have been REMOVED from Group " );
            user_index    =        "MEMBERSHIPS.GROUPS"                 ;
            break;

         case ACTIVITY_OBJ :

            s_init( &  add_ntf,   "You have been ADDED to Activity "   );
            s_init( &  tmpstr ,   " in Activity "                      );
            s_init( &  rem_ntf,  "You have been REMOVED from Activity ");
            user_index    =      "INDEXES.ACTIVITIES"                   ;
            break;

         default :

            rc = INVALID_OCB;
            break;

      }

      s_cat(   & add_ntf, add_ntf, owner_id,   0, 0 );

      s_init(  & mod_ntf, "Your role has been MODIFIED to ");
      s_cat (  & mod_ntf, mod_ntf, memb_roles, 0, 0);
      s_cat (  & mod_ntf, mod_ntf, tmpstr,     0, 0);
      s_cat (  & mod_ntf, mod_ntf, owner_id,   0, 0 );

      s_cat(   & rem_ntf, rem_ntf, owner_id,   0, 0 );

      s_drop(  & tmpstr );
      s_alloc( & tmpstr, s_len(add_ntf) + s_len(members_to_add)
                                                      + 2 * S_OVERHEAD );
      s_catb(  & tmpstr, (byte *) add_ntf, s_len(add_ntf) + S_OVERHEAD );
      s_catb(  & tmpstr, (byte *) members_to_add, s_len(members_to_add)
                                                          + S_OVERHEAD );
      s_drop(  & add_ntf );
      add_ntf  = tmpstr;
      tmpstr   = S_NULL;

      s_alloc( & tmpstr, s_len(mod_ntf) + s_len(members_to_modify)
                                                      + 2 * S_OVERHEAD );
      s_catb(  & tmpstr, (byte *) mod_ntf, s_len(mod_ntf) + S_OVERHEAD );
      s_catb(  & tmpstr, (byte *) members_to_modify, s_len(members_to_modify)
                                                          + S_OVERHEAD );
      s_drop(  & mod_ntf );
      mod_ntf  = tmpstr;
      tmpstr   = S_NULL;

      s_alloc( & tmpstr, s_len(rem_ntf) + s_len(members_to_remove)
                                                      + 2 * S_OVERHEAD );
      s_catb(  & tmpstr, (byte *) rem_ntf, s_len(rem_ntf) + S_OVERHEAD );
      s_catb(  & tmpstr, (byte *) members_to_remove, s_len(members_to_remove)
                                                          + S_OVERHEAD );
      s_drop(  & rem_ntf );
      rem_ntf  = tmpstr;
      tmpstr   = S_NULL;



      ocd_new( & memb_ocd, "MEMBERSHIP.COUNT" );
      obj_get(   memb_ocb, memb_ocd,  & count );
      count   += (am_count - rm_count);
      obj_rep(   memb_ocb, memb_ocd,  & count, SEG_UPDATE );

      count = 0;
      ocd_new( & memb_ocd, "MEMBERSHIP.WRITER" );
      obj_get(   memb_ocb, memb_ocd,   & count );
      count   += (am_count - rm_count);
      obj_rep(   memb_ocb, memb_ocd,   & count, SEG_UPDATE );

      os_dttm( & cur_dt );

      s_init(  & roles_ocd,  "MEMBERSHIP.MEMBERS.ROLES"                );
      s_init(  & date_ocd,   "MEMBERSHIP.MEMBERS.ADD_DATE"             );
      s_init(  & root_ocd,   "MEMBERSHIP.MEMBERS.VARIABLES.ROOT_RANGE" );
      s_init(  & active_ocd, "MEMBERSHIP.MEMBERS.LAST_ACTIVE"          );

      s_init(  & (memb_ocd->name), "MEMBERSHIP.MEMBERS" );
      tmp_mocd =  memb_ocd->name ;
      s_alloc( & (memb_ocd->key ),   sizeof( OID ) );
      s_smp_ul(   memb_ocd->key ) =  sizeof( OID );
      oidkey   = (OID *) s_buffer( memb_ocd->key );

      s_init( & (list_ocd->name), "LIST.LIST.OID" );
      list_ocd->bias = 1;


      for(  i = 0;  (i != am_count) && (rc == NO_ERROR);  i++  )
      {

         rc = obj_get( add_memb_list, list_ocd, oidkey );


         memb_ocd->name = roles_ocd;
         rc = obj_rep( memb_ocb, memb_ocd, s_buffer(memb_roles),
                                                            SEG_UPDATE );

         if( rc != NO_ERROR )
         {

            continue;

         }
         memb_ocd->name = date_ocd;
         rc = obj_rep(memb_ocb, memb_ocd, & cur_dt, SEG_UPDATE);
         memb_ocd->name = active_ocd;
         os_memn( & last_active, sizeof( datetime ) );
         last_active.century = 19;
         rc = obj_rep( memb_ocb, memb_ocd, & last_active, SEG_UPDATE );

         if( rc != NO_ERROR )
         {

            continue;

         }
         memb_ocd->name = root_ocd;
         rc = obj_rep(memb_ocb, memb_ocd, & memb_root, SEG_UPDATE);

         if( rc != NO_ERROR )
         {

            continue;

         }

         user_ocb = obj_opn( oidkey, OBJ_UPDATE );
         if( ! OCB_POINTER_OK( user_ocb ) )
         {

            rc = INVALID_OCB;
            continue;

         }
         else
         {

            string     oid_key = S_NULL;

            s_alloc( & oid_key,                        sizeof( OID ) );
            os_memc( & owner_oid, s_buffer( oid_key ), sizeof( OID ) );
            s_smp_ul(  oid_key ) =                     sizeof( OID )  ;
            rc = add_to_user_list( user_ocb, user_index, oid_key,
                                                & owner_oid, 0, S_NULL );
            s_drop(  & oid_key );
            obj_cls(  user_ocb );

         }
      }

      if( (am_count != 0) && (rc == NO_ERROR) )
      {

        rc = cr_ntf( memb_ocb, add_ntf, AUTOMATIC_CREATION, & add_memb_list);

      }

                                  /* Now lets work with modifing users */

      s_init( & date_ocd,   "MEMBERSHIP.MEMBERS.LAST_MODIFIED");

      for(  i = 0;  (i != mm_count) && (rc == NO_ERROR);  i++  )
      {

         obj_get( modify_memb_list, list_ocd, oidkey );

         memb_ocd->name = roles_ocd;
         rc= obj_rep(memb_ocb,memb_ocd,s_buffer(memb_roles),SEG_HOLD);

         if( rc != NO_ERROR )      /* Get out if there was an err  */
         {

            continue;

         }

         memb_ocd->name = date_ocd;
         rc = obj_rep(memb_ocb, memb_ocd, & cur_dt, SEG_UPDATE);

         if( rc != NO_ERROR )      /* Get out if there was an err  */
         {

            continue;

         }

         memb_ocd->name = root_ocd;
         rc = obj_rep(memb_ocb, memb_ocd, & memb_root, SEG_UPDATE);

         user_ocb = obj_opn( oidkey, OBJ_UPDATE );

         if( OCB_POINTER_OK( user_ocb ) &&  (rc == NO_ERROR) )
         {

            obj_cls( user_ocb );

         }
      }   /*  for modify members  */

                                  /* If there was any modification and */
                                  /* we're not doing linking since we  */
                                  /* only want to notify people added  */
      if( (mm_count != 0) &&   (rc    == NO_ERROR)
                          &&  ((level &  MEMB_DOING_LINKING) == 0) )
      {

        rc = cr_ntf( memb_ocb, mod_ntf, AUTOMATIC_CREATION, & modify_memb_list);

      }
                            /* Now delete users                  */

                                    /* Get ready to remove from  */
      memb_ocd->name = tmp_mocd;    /* MEMBERS index             */

      for( i = 0;  (i != rm_count) && (rc == NO_ERROR); i++ )
      {

         obj_get( remove_memb_list, list_ocd, oidkey );

         rc = obj_rmv( memb_ocb, memb_ocd );

         if( rc != NO_ERROR )      /* Get out if there was an err  */
         {

            continue;

         }
                                 /* Now lets update USER object  */
                                 /* First, open the OCB then     */
                                 /* update the appropriate count */
                                 /* and remove from index and    */
                                 /* finally close the OCB        */

         user_ocb = obj_opn(oidkey, OBJ_UPDATE );
         if( OCB_POINTER_OK( user_ocb ) == FALSE )
         {
            rc = INVALID_OCB;    /* error or?????                */
            continue;            /* work on next user            */

         }
         else
         {

            user_ocd->name = usr_count;
            rc = obj_get(    user_ocb, user_ocd, & count );
            if( rc == NO_ERROR )
            {

               count--;
               obj_rep( user_ocb, user_ocd, & count, SEG_UPDATE );

               s_init( &(user_ocd->name), user_index );
               obj_rmv(  user_ocb,        user_ocd   );

            }

                            /* Should we try to close the object */
                            /* when OCB_POINTER_OK() says it is  */
                            /* bad?? If so, move this out of else*/
            obj_cls( user_ocb );

         }
      }  /* for remove members */

                                  /* If there was any deletion and     */
                                  /* we're not doing linking since we  */
                                  /* only want to notify people added  */
      if( (rm_count != 0) &&  (rc    == NO_ERROR)
                          && ((level &  MEMB_DOING_LINKING) == 0) )
      {

        rc = cr_ntf( memb_ocb, rem_ntf, AUTOMATIC_CREATION, & remove_memb_list);

      }
                               /* Now update TMP.CONTENTS to reflect*/
                               /* changes made in this routine      */

      if(    ((am_count != 0) || (mm_count != 0) || (rm_count != 0))
          &&  ( rc == NO_ERROR)   )
      {

         rc = memb_fmt( memb_ocb );

      }
      if( OCD_POINTER_OK( memb_ocd ) )
      {

         memb_ocd->key  =   S_NULL;
         memb_ocd->name = tmp_mocd;
         ocd_drp( & memb_ocd     );

      }
      if( OCD_POINTER_OK( user_ocd ) )
      {

         user_ocd->name = tmp_uocd;
         user_ocd->key  =   S_NULL;
         ocd_drp( & user_ocd     );

      }
   }  /* else if there were no errors while validating names */

                                /* + + + + + + + + + + + + + + + + + + */
                                /* If we are doing creation then we do */
                                /* NOT want to delete the user lists   */
                                /* unless there are errors.            */
                                /* + + + + + + + + + + + + + + + + + + */

   if( (MEMB_DOING_LINKING  & level) && (rc == NO_ERROR) )
   {


      if( OCB_POINTER_OK( add_memb_list ) )
      {

          obj_set_flag( add_memb_list, OBJ_DELETE );
          obj_update( memb_ocb, "TMP.ADDOID", NIL );

      }
      if( OCB_POINTER_OK( modify_memb_list ) )
      {

          obj_set_flag( modify_memb_list, OBJ_DELETE );
          obj_update( memb_ocb, "TMP.MODOID", NIL     );

      }
      if( OCB_POINTER_OK( remove_memb_list ) )
      {

          obj_set_flag( remove_memb_list, OBJ_DELETE );
          obj_update( memb_ocb, "TMP.REMOID", NIL    );

      }
   }  /* delete list objects */


   if( OCB_POINTER_OK( add_memb_list ) )
   {

      obj_cls( add_memb_list );

   }
   if( OCB_POINTER_OK( modify_memb_list ) )
   {

      obj_cls( modify_memb_list );

   }
   if( OCB_POINTER_OK( remove_memb_list ) )
   {

      obj_cls( remove_memb_list );

   }

   s_drop(  &  owner_id   );
   s_drop(  &  add_err    );
   s_drop(  &  mod_err    );
   s_drop(  &  rem_err    );
   s_drop(  &  roles_ocd  );
   s_drop(  &  root_ocd   );
   s_drop(  &  date_ocd   );
   s_drop(  &  active_ocd );
   s_drop(  &  usr_count  );
   s_drop(  &  add_ntf    );
   s_drop(  &  mod_ntf    );
   s_drop(  &  rem_ntf    );
   s_drop(  &  tmpstr     );
   ocd_drp( &  list_ocd   );


/* if(  (level & MEMB_DOING_LINKING)  &&  (rc == NO_ERROR)  ) */
   if(                                    (rc == NO_ERROR)  )
   {

      rc =
         t_m_obj( t_invoker(), memb_ocb, (void **) result_str, FALSE, 0);

   }
   else
   {

      rc = t_csend( t_invoker(), rc, * result_str );

   }
   TM_master->current->except = rc;
   rc = DO_NOT_CONFIRM_TRANSACTION;

}  /* if on MS */

   return(rc);

}
