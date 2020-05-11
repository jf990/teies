/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    cr_cr()                                                |
  |  <AUTHOR>  John L. Foster, Rana Ajaz-Ur-Rehman, Sal Johar         |
  |  <DATE>    June 23, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   ocb         OCB *      INPUT  not used.                |
  |  <PARAM>   modifier    string     INPUT  not  used.               |
  |  <PARAM>   level       hword      INPUT  not used.                |
  |  <PARAM>   param       byte *     INOUT  not used.                |
  |                                                                   |
  |  <PURPOSE> Create Conference Root.                                |
  |            This routine allocates the object for conference root  |
  |            and serves the pupose of controlling the screen flow   |
  |            logic of the forms involved in the process. Depending  |
  |            upon the return value of each routine called, the next |
  |            routine is called.                                     |
  |                                                                   |
  |            The decision as to which routines will be called       |
  |            from here basically depends upon the user's selection  |
  |            on the first screen (3140 form), which is called up    |
  |            by the first routine called from here (i.e.            |
  |            CFR_INFO()) after allocating the object.               |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   obj_all(), cfr_info(), cfr_gtreq(), cfr_gtpub(),       |
  |            cfr_gtdes(), cfr_cfrm(), cr_memb(), lk_cr(),           |
  |            obj_cls(), cfr_clear()                                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"
#include "obj_memb.h"
#include "obj_cr.h"




sint cr_cr( ocb, modifier, level, params )

OCB      *  ocb;
string      modifier;
hword       level;
byte     *  params;

{

sint     next_step      = 0,
         last_id        = 0,
         rc             = 0;

string   conf_id        = S_NULL;

hword    to_do_list     = 0,
         done_list      = 0,
         memb_level     = MEMB_DOING_CREATION;

OCD      *user_ocd      = NULL;

OCB      *conf_ocb      = NULL,
         *user_ocb      = NULL,
         *memb_ocb      = NULL;

boolean  linked         = FALSE,
         done           = FALSE;




conf_ocb = obj_all( CONF_OBJ, LOCAL_OBJECT, NIL );

if( OCB_POINTER_OK( conf_ocb ) )
{


next_step =  STARTING_POINT_CONFERENCE_CREATE;


while( ! done )
{

   switch ( next_step )
   {

   case  STARTING_POINT_CONFERENCE_CREATE :

      to_do_list = 0;             /* Nothing to do in begining         */
      rc = cfr_info( NULL, S_NULL, done_list, & to_do_list );
      break;

   case  REQUIRED_INFORMATION :

      rc = cfr_gtreq( conf_ocb, S_NULL, level, NULL );
      break;

   case  PUBLIC_INFORMATION :

      rc = cfr_gtpub( conf_ocb, S_NULL, level, NULL );
      break;

   case  DESCRIPTION_INFORMATION :

      rc = cfr_gtdes( conf_ocb, S_NULL, level, NULL );
      break;

   case  ASSIGNMENT_INFORMATION :

      rc = cr_memb( conf_ocb, S_NULL, memb_level, & memb_ocb );
      if( rc == NO_ERR )
      {                          /* So next time we call cr_memb(), it */
                                 /* does not allocate a new object     */
         memb_level |= MEMB_USE_EXISTING_OBJECT;

      }
      break;

   case  OK_TO_CREATE_CONFERENCE :

      rc = lk_cr( conf_ocb, S_NULL, (hword) 0, S_NULL );
      if( rc == NO_ERR )
      {

         to_do_list |= DISPLAY_CONFERENCE_CREATED;

                                    /* Following code should be taken  */
                                    /* out once we have updated view   */
                                    /* of user object after LK_CR      */
         user_ocb = TM_master->ucb->directory;

         ocd_new( & user_ocd, "MEMBERSHIPS.LAST_CONF_ID" );
         obj_get(   user_ocb, user_ocd, & last_id );
         last_id++;
         obj_rep(   user_ocb, user_ocd, & last_id, SEG_UPDATE );

         last_id = 0;
         s_init ( &(user_ocd->name), "MEMBERSHIPS.OWNER_CONF_C" );
         obj_get( user_ocb, user_ocd, &last_id );
         last_id++;
         obj_rep( user_ocb, user_ocd, &last_id, SEG_UPDATE );

         s_init ( &(user_ocd->name), "CONFERENCE.ID" );
         obj_get( conf_ocb, user_ocd, &conf_id );

         s_init ( &(user_ocd->name), "MEMBERSHIPS.CONF_OWNER.OID" );
         user_ocd->key = conf_id;
         obj_rep( user_ocb, user_ocd, &(conf_ocb->oid), SEG_UPDATE );

         ocd_drp( &user_ocd );         /* Also droping conf_id         */
         user_ocd = NULL;

      }
      break;

   case  DISPLAY_CONFERENCE_CREATED :

      rc = cfr_cfrm( conf_ocb, S_NULL, (hword) 0, &to_do_list );
      break;

   case  CREATE_ANOTHER_CONFERENCE :

      cfr_clear( NULL, S_NULL, 0, NULL );

      obj_set_flag( conf_ocb, OBJ_DELETE );
      obj_cls( conf_ocb );
      conf_ocb = NULL;
      if( OCB_POINTER_OK( memb_ocb ) )
      {

         obj_set_flag( memb_ocb, OBJ_DELETE );
         obj_cls( memb_ocb );
         memb_ocb = NULL;

      }

      conf_ocb = obj_all( CONF_OBJ, LOCAL_OBJECT, NIL   );

      memb_level &= ~MEMB_USE_EXISTING_OBJECT;   /* Don't need this flag    */
      done_list = 0;                        /* No need for this now    */

      break;

   case  ABANDON_CREATION :
   case  CLEAN_UP         :

      obj_set_flag( conf_ocb, OBJ_DELETE );
      obj_cls( conf_ocb );
      if( OCB_POINTER_OK( memb_ocb ) )
      {

         obj_set_flag( memb_ocb, OBJ_DELETE );
         obj_cls( memb_ocb );

      }

      cfr_clear(NULL, S_NULL, 0, NULL );

      done = TRUE;
      break;

   default :

      done = TRUE;
      rc = NEXT_STEP_NOT_DEFINED_IN_CR_CR;

   }  /* switch */

   if( rc == NO_ERR )
   {

      to_do_list &= ~next_step;  /* We're over with this step, turn off*/
      done_list  |=  next_step;  /* to_do and turn on done flags       */

   }

                                  /* * * * * * * * * * * * * * * * * * */
   if( ! done )                   /* If we're not DONE with creation   */
   {                              /* process then figure out what we   */
                                  /* our next_step should be!          */
                                  /* There HAS to be better way to     */
                                  /* find next_step, but I can't think */
                                  /* of it right now!                  */
                                  /* * * * * * * * * * * * * * * * * * */
      if( rc == USER_REQUESTED_QUIT )
      {

         if( next_step == STARTING_POINT_CONFERENCE_CREATE )
         {

            next_step = ABANDON_CREATION;

         }
         else
         if( next_step == DISPLAY_CONFERENCE_CREATED )
         {

            next_step = CLEAN_UP;

         }
         else
         {

            next_step = STARTING_POINT_CONFERENCE_CREATE;

         }

      }
      else
      if( rc != 0 )               /* If there is any other error, just */
      {                           /* abandon everything.               */

          next_step = ABANDON_CREATION;

      }
      else
      if( to_do_list & REQUIRED_INFORMATION )
      {

         next_step = REQUIRED_INFORMATION;

      }
      else
      if( to_do_list & PUBLIC_INFORMATION )
      {

         next_step = PUBLIC_INFORMATION;

      }
      else
      if( to_do_list & DESCRIPTION_INFORMATION )
      {

         next_step = DESCRIPTION_INFORMATION;

      }
      else
      if( to_do_list & ASSIGNMENT_INFORMATION )
      {

         next_step = ASSIGNMENT_INFORMATION;

      }
      else
      if( to_do_list & OK_TO_CREATE_CONFERENCE )
      {

         next_step = OK_TO_CREATE_CONFERENCE;

      }
      else
      if( to_do_list & DISPLAY_CONFERENCE_CREATED )
      {

         next_step = DISPLAY_CONFERENCE_CREATED;

      }
      else
      if( to_do_list & CREATE_ANOTHER_CONFERENCE )
      {

         next_step = CREATE_ANOTHER_CONFERENCE;

      }
      else
      if( to_do_list == 0 )       /* If we're done with what we were   */
      {                           /* asked to do by the user           */

         if( done_list & OK_TO_CREATE_CONFERENCE )
         {                        /* And if user had said ok to create */
                                  /* then we're done with everything   */
            next_step = CLEAN_UP;

         }
         else
         {

            next_step = STARTING_POINT_CONFERENCE_CREATE;

         }

      }
      else
      {                       /* We SHOULD have all the states covered */
                              /* but just in case, lets put this defaul*/
                              /* Remove it once we have a stable versio*/
         rc = NEXT_STEP_NOT_DEFINED_IN_CR_CR;

      }

   }     /* If we're already done */

}    /* while NOT done interacting with the user */

}
else
{

   rc = INVALID_OCB;

}


   return(rc);

}
