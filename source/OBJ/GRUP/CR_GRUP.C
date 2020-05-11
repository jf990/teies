/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    CR_Grup
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman, Tanmay S. Kumar   |
  |  <DATE>    August 30, 1988                      August 15, 1989   |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT  not used.                |
  |  <PARAM>   modifier    string     INPUT  not  used.               |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   param       byte *     INOUT  not used.                |
  |                                                                   |
  |  <PURPOSE> Create Group.                                          |
  |            This routine allocates the object for a new GROUP      |
  |            and serves the pupose of controlling the screen flow   |
  |            logic of the forms involved in the process. Depending  |
  |            upon the return value of each routine called, the next |
  |            routine is called.                                     |
  |                                                                   |
  |            The decision as to which routines will be called       |
  |            from here basically depends upon the user's selection  |
  |            on the first screen (3120 form), which is called up    |
  |            by the first routine called from here (i.e. cr_grup1())|
  |            after allocating the object.                           |
  |  <OUTPUT>  Sint                                                   |
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

sint cr_grup( ocb, modifier, level, params )

OCB      *ocb;
string   modifier;
hword    level;
byte     *params;

{


sint     next_step      =        0,
         rc             =        0,
         last_id        =        0;

string   group_id       =   S_NULL,
         sp_string      =   S_NULL;

hword    to_do_list     =        0,
         done_list      =        0,
         memb_level     = MEMB_DOING_CREATION;

OCD      *user_ocd      =      NIL;

OCB      *group_ocb     =      NIL,
         *memb_ocb      =      NIL,
         *user_ocb      =      NIL;

boolean  linked         =    FALSE,
         done           =    FALSE;

#D_begin( cr_grup, OBJ );

   #D_say('Allocating Group object'                  );
   group_ocb = obj_all( GROUP_OBJ, LOCAL_OBJECT, NIL   );
   #D_ocb('group_ocb');
   next_step = STARTING_POINT_GROUP_CREATE;

   while( ! done )
   {

      #D_show('Next Step is 0x%x; To_do = 0x%x; Done = 0x%x', \
                                  'next_step, to_do_list, done_list')
      switch ( next_step )
      {

         case  STARTING_POINT_GROUP_CREATE :

            #D_say('GOING TO CALL grp_srt()');
            to_do_list = 0;     /* Nothing to do in begining         */
            rc = grp_srt( NIL, S_NULL, done_list, &to_do_list  );
            break;

         case  REQUIRED_INFORMATION :

            #D_say('Fill in required information')
            rc = grp_req( group_ocb, S_NULL, level, NULL );
            break;

         case  PUBLIC_INFORMATION :

            #D_say('Fill in public information')
            rc = grp_pub( group_ocb, S_NULL, level, NULL );
            break;

         case  DESCRIPTION_INFORMATION :

            #D_say('Fill in descriptive information')
            rc = grp_des( group_ocb, S_NULL, level, (byte *) &sp_string);
            break;

         case  ASSIGNMENT_INFORMATION :

            #D_say('Fill in membership information')
            rc = cr_memb( group_ocb, S_NULL, memb_level, &memb_ocb );
            if( rc == NO_ERR )
            {                    /* So next time we call cr_memb(), it */
                                 /* does not allocate a new object     */
               memb_level |= MEMB_USE_EXISTING_OBJECT;

            }
            break;

         case  OK_TO_CREATE_GROUP :

            #D_say('Now create our GROUP on MS')
            #D_str('sp_string', 'sp_string');
            t_delsp( sp_string );
            rc = lk_grup( group_ocb, S_NULL, (hword) 0, S_NULL   );
            if( rc == NO_ERR )
            {

               to_do_list |= DISPLAY_GROUP_CREATED;

                                    /* Following code should be taken  */
                                    /* out once we have updated view   */
                                    /* of user object after LK_GRUP    */
               user_ocb = TM_master->ucb->directory;

               ocd_new( &user_ocd, "MEMBERSHIPS.LAST_GROUP_ID" );
               obj_get( user_ocb, user_ocd, &last_id );
               last_id++;
               obj_rep( user_ocb, user_ocd, &last_id, SEG_UPDATE );

               last_id = 0;
               s_init ( &(user_ocd->name), "MEMBERSHIPS.OWNER_GROUP_C");
               obj_get( user_ocb, user_ocd, &last_id );
               last_id++;
               obj_rep( user_ocb, user_ocd, &last_id, SEG_UPDATE );

               s_init ( &(user_ocd->name), "GROUP.ID" );
               obj_get( group_ocb, user_ocd, &group_id );

               s_init (&(user_ocd->name),"MEMBERSHIPS.GROUP_OWNER.OID");
               user_ocd->key = group_id;
               obj_rep(user_ocb,user_ocd,&(group_ocb->oid),SEG_UPDATE );

               ocd_drp( &user_ocd );   /* Also droping group_id */
               user_ocd = NULL;

            }
            break;

         case  DISPLAY_GROUP_CREATED :

            #D_say('group_creation went ok...want to create another')
            rc = grp_dis( group_ocb, NULL, (hword) 0, &to_do_list );
            break;

         case  CREATE_ANOTHER_GROUP :

            #D_say('Create new group: clear out all the forms vars')
            grp_clr( NULL, S_NULL, 0, NULL );

            #D_say('Closing objects now')
            obj_cls( group_ocb );
            if( OCB_POINTER_OK( memb_ocb ) )
            {
               obj_cls( memb_ocb );
            }

            group_ocb = NULL;
            group_ocb = obj_all( GROUP_OBJ, LOCAL_OBJECT, NIL   );
            #D_show('New group_ocb_ptr=0x%X', 'group_ocb'  );
            #D_ocb('group_ocb');
            memb_ocb = NULL;
            memb_level &= ~MEMB_USE_EXISTING_OBJECT;
                                            /* Don't need this flag    */
            done_list = 0;                /* No need for this now    */

            break;

         case  ABANDON_CREATION :

            #D_say('erasing everything we created')
            #D_ocb('group_ocb')
            obj_set_flag( group_ocb, OBJ_DELETE );
            #D_ocb('memb_ocb')
            if( OCB_POINTER_OK( memb_ocb ) )
            {
                obj_set_flag( memb_ocb, OBJ_DELETE );
            }
                                 /* Now fall into closeing everything */
         case  CLEAN_UP :

            #D_say('Closing everything now')
            obj_cls( group_ocb );
            if( OCB_POINTER_OK( memb_ocb ) )
            {
               obj_cls( memb_ocb );
            }

            grp_clr( NULL, S_NULL, 0, NULL );

            done = TRUE;
            break;

         default :

            done = TRUE;
            rc = NEXT_STEP_NOT_DEFINED_IN_CR_CR;

      }  /* switch */

      if( rc == NO_ERR )
      {

         /* We're over with this step, turn off*/
         #D_show('Next Step is 0x%x; To_do = 0x%x; Done = 0x%x', \
                                  'next_step, to_do_list, done_list')
         to_do_list &= ~next_step;
         /* to_do and turn on done flags       */
         done_list  |=  next_step;
         #D_show('Next Step is 0x%x; To_do = 0x%x; Done = 0x%x', \
                                  'next_step, to_do_list, done_list')

      }

      if( !done )              /* If we're not DONE with creation   */
      {                        /* process then figure out what we   */
                               /* our next_step should be!          */
                               /* There HAS to be better way to     */
                               /* find next_step, but I can't think */
                               /* of it right now!                  */
         if( rc == USER_REQUESTED_QUIT )
         {
            if( next_step == STARTING_POINT_GROUP_CREATE )
            {
               next_step = ABANDON_CREATION;
            }
            else if( next_step == DISPLAY_GROUP_CREATED )
            {
               next_step = CLEAN_UP;
            }
            else
            {
               next_step = STARTING_POINT_GROUP_CREATE;
            }
         }
         else if (rc != 0)
                               /* If there is any other error, just */
         {                     /* abandon everything.               */
            next_step = ABANDON_CREATION;
         }
         else if( to_do_list & REQUIRED_INFORMATION )
         {
            next_step = REQUIRED_INFORMATION;
         }
         else if( to_do_list & PUBLIC_INFORMATION )
         {
            next_step = PUBLIC_INFORMATION;
         }
         else if( to_do_list & DESCRIPTION_INFORMATION )
         {
            next_step = DESCRIPTION_INFORMATION;
         }
         else if( to_do_list & ASSIGNMENT_INFORMATION )
         {
            next_step = ASSIGNMENT_INFORMATION;
         }
         else if( to_do_list & OK_TO_CREATE_GROUP )
         {
            next_step = OK_TO_CREATE_GROUP;
         }
         else if( to_do_list & DISPLAY_GROUP_CREATED )
         {
            next_step = DISPLAY_GROUP_CREATED;
         }
         else if( to_do_list & CREATE_ANOTHER_GROUP )
         {
            next_step = CREATE_ANOTHER_GROUP;
         }
         else if( to_do_list == 0 )
         {
             /* If we're done with what we were   */
             /* asked to do by the user           */

            if( done_list & OK_TO_CREATE_GROUP )
            {
                /* And if user had said ok to create */
                /* then we're done with everything   */
               next_step = CLEAN_UP;

            }
            else
            {
               next_step = STARTING_POINT_GROUP_CREATE;
            }

         }
         else
         {
                           /* We SHOULD have all the states covered */
                           /* but just in case, lets put this defaul*/
                           /* Remove it once we have a stable versio*/
            rc = NEXT_STEP_NOT_DEFINED_IN_CR_CR;

         }

      }  /* If we're already done */

   }  /* while */


#D_leave( rc, 'Done with Group creation' )

}
