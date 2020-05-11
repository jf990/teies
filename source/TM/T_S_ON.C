/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
<DOC>
<NAME>            T_S_ON()
<LEVEL>           TM
<AUTHOR>          John L. Foster
<DATE>            June 12, 1987
<PARAM>           type_of_signon  SINT  INPUT   What type of signon is
                                                requested.
<OUTPUT>          rc   sint
<GLOBALS>         TM_master

<PURPOSE>
       Signon a user on to TEIES, indicating to the local Master
       of this user's presence in the system.  Procedure is 1) call the
       MS with a signon request; 2) wait for MS to respond.  If confirm
       is OK then the response packet has the users object in it. Else
       we should post a logon screen to get the users TEIES name and
       access code.  Repeat steps 1 and 2 until success.

                  MS is online                    MS is not online
                        v                                v
                        |                                |
    User is *not*       |     User *is* a                |
    a TEIES member      |     TEIES member               |
                        |
              +---------+-----------+                SIGNONSU
              |                     |
              |                     |                    |
                                                         |
            LOGON            +-> SIGNON                  |
                             |                           |
              |              |      |                    |
              | User *is* a  |      |                    |
              | TEIES member |      |                    |
              +--------------+      +---------+----------+
              |                               |
              |                               |
              |Not a TEIES                    v
              |member                      Homebase
              v
           end TEIES


           NOTE: Usually in the current state we figure out the next
           state to jump to.  Not here.  After signon we call Homebase
           and let Homebase logic figure out its own state since there
           can be a signon profile to locate and different user interface
           levels to maintain.

           If type_of_signon is TM_SECONDARY_SIGNON then force the
           signon screen and do not run the user determination code.
           This will allow multiple signons in a single TEIES session.

<ERRORS>
              20007 : A master server was not found.
              20008 : MS connection established but cannot send data.
              28833 : Quit of TEIES requested on a signon form.
              28888 : MasterServer finished/shutdown request.

<CALLS>       isf_open(), except(), isf_send(), t_iscf_t()
<FORMS>       LOGON, SIGNON, SIGNONSU
<LIMITS>
<MODS>        May 8, 1989    JF      Added type_of_signon parameter and
                                     additional signon logic.
</DOC>
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"
#include "obj_user.h"
#include "obj_name.h"
#include "isv.h"







sint     t_s_on( type_of_signon )

sint             type_of_signon;

{

sint     rc        =      0,
         spare     =      0,
         watchdog  =      0,
         mail_count=      0,
         user_status=     0,
         state_id  =      0,
         request   =      0;
string
         CREATED   = S_NULL,
         MODIFIED  = S_NULL,
         RECEIVED  = S_NULL,
         stat_entry= S_NULL,
         dtme      = S_NULL,
         dt        = S_NULL,
         state     = S_NULL,
         access_mth= S_NULL,
         name      = S_NULL,
         laston    = S_NULL,
         convert   = S_NULL;

hword    command   =      0,
         flags     =      0;
boolean  ok        =  FALSE,
         connected =   TRUE;
OCB     *userocb   =    NIL,
        *psi_ocb   =    NIL;
OCD     *ocd       =    NIL,
        *psi_ocd   =    NIL;
OID      user_oid          ;
datetime last_on_dt        ;

USER_STAT_ENTRY * stat_record = NIL;





   TM_master->flags |= IN_SIGNON;
   s_dttos( & dt, &(TM_master->ucb->time_on), (TM_master->ucb->dtform) );
   var_init( & dtme,       "%TIMEON", dt,      OC_STR  );
   var_init( & access_mth, "%ACCESS", "Local", OC_CSTR );

/* ==> We should call os_lninfo() to get the terminal access method<== */


                             /* = = = = = = = = = = = = = = = = = = = *\
                              |If the user's object is stored locally,|
                              |access it now.  Then if the connection |
                              |is opened update its contents.         |
                             \* = = = = = = = = = = = = = = = = = = = */

psi_ocb = t_sysndx(         PSI_OBJ        );
ocd_new( &  psi_ocd,       "PSI.INDEX.OID" );
s_init(  & (psi_ocd->key), "IDENTITY"      );
rc = obj_get( psi_ocb, psi_ocd, & user_oid );
if( rc == 0 )
{

   userocb = obj_opn( & user_oid, OBJ_UPDATE );
   userocb->open_count ++ ;
   TM_master->ucb->directory = userocb;

}

                                            /***************************/
                                            /*                         */
if( TM_master->usrtype != MASTER_SERVER )   /* Open a connection       */
{                                           /* to the master server,   */
                                            /* and then send a request */
   rc = t_opncon(TM_master->MS,             /* for signon              */
                 MASTER_SERVER );           /*                         */
                                            /***************************/
   if( rc == 0 || rc == Connection_Already_In_Progress
               || rc == Connection_Timeout              )
   {

      string signon_req = S_NULL;


      s_init( & signon_req, "SIGNON" );
      rc = t_send(  TM_master->MS, SIGNON, flags, state_id, signon_req );
      s_drop( & signon_req );

      if( rc != 0 )
      {

         t_cls_t( TM_master->MS, CLOSE_CONNECTION );
         connected = FALSE;

      }
      if( connected )
      {

         rc = t_waitc( TM_master->MS,
                               (void **)  &(TM_master->ucb->directory) );
                                                /***********************/
                                                /* Now we asked MS to  */
                                                /* signon, so wait for */
                                                /* a response. If OK,  */
                                                /* then request the    */
                                                /* directory entry for */
                                                /* this user.          */
                                                /***********************/
         if( rc != 0 )                   /******************************/
         {                               /*If the flags sent back are  */
                                         /*not 0, then                 */
                                         /*this user ID is not a TEIES */
                                         /*member.  Give a signon scrn */
                                         /*to try a valid user.        */
                                         /******************************/

            string
                   packet   = S_NULL,
                   user_id  = S_NULL,
                   hold_str = S_NULL,
                   password = S_NULL;

            s_init( & state,    "LOGON"    );
            s_init( & user_id,  "USER_ID"  );
            s_init( & password, "PASSWORD" );
            while( TRUE )
            {

               request = t_form( state, NIL, NIL );

               if( request == USER_REQUESTED_ABANDON )
               {

                  rc = SIGNOFF_REQUESTED ;
                  connected = FALSE;
                  break;

               }
               else
               {

                  if( request == USER_REQUESTED_QUIT )
                  {

                     connected = FALSE;
                     break;

                  }
                  hold_str = (string) var_get( user_id );
                  s_upper(  hold_str,         0, 0     );
                  s_catx( & packet, hold_str, 0, 0     );
                  hold_str = (string) var_get( password);
                  s_upper(  hold_str,         0, 0     );
                  s_catx( & packet, hold_str, 0, 0     );
                  command = SIGNON_VERIFY;
                  flags   = 0;
                  rc = t_send( TM_master->MS, command, flags, state_id, packet);
                  if( rc == 0 )
                  {

                     rc = t_waitc( TM_master->MS,
                               (void **)  &(TM_master->ucb->directory) );

                /*+---------------------------------------------------+
                  |If RC == 0 then the master server is telling us the|
                  |name/password combination did not match.  For this |
                  |version if it fails go into single user mode rather|
                  |than reiterate the log-on sequence.                |
                  +---------------------------------------------------+*/
                     if( rc != 0 )
                     {

                        connected = FALSE;
                        if( rc == System_ID_Not_In_SNI
                        ||  rc == Name_Not_in_SNI
                        ||  rc == Not_A_TEIES_User_ID
                        ||  rc == Password_Does_Not_Match_Name  )
                        {

                           rc = Name_Not_in_SNI;

                        }
                        except( rc, USER_INFO, var_get(user_id), S_NULL,
                                                S_NULL, S_NULL, S_NULL );
                        f_rstfld( state, STATE_FORM, password );

                     }
                     else
                     {

                        connected = TRUE;
                        break;

                     }
                  }
                  else
                  {

                     t_cls_t( TM_master->MS, CLOSE_CONNECTION );
                     connected = FALSE;

                  }
                  s_drop( & packet   );

               }
            }
            s_drop( & user_id  );
            s_drop( & password );
            f_close( state, STATE_FORM );

         }
      }
   }
   else
   {

      connected = FALSE;

   }
   if( ! connected )
   {

      if( rc != SIGNOFF_REQUESTED )
      {

         except( MASTER_SERVER_IS_NOT_AVAILABLE, CONTROL_ERROR,
                 TM_master->MS, S_NULL, S_NULL, S_NULL, S_NULL      );
         s_init( & state, "SIGNONSU");
         TM_master->ucb->directory = NIL ;

         request = t_form( state, NIL, NIL );
         rc = 0;

         if( request == USER_REQUESTED_ABANDON )
         {

            rc = SIGNOFF_REQUESTED ;

         }
      }
   }
   else
   {

   /*+- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+
     |                                                               |
     | We are connected to a Master Server and our User Directory    |
     | has already been send back and placed in our local DB.  Now   |
     | update local TM UCB pointers so the user data is readily      |
     | accessbile.  Ready user data for the signon screen.           |
     |                                                               |
     +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -+*/

      TM_master->flags |= CONNECTED_TO_MASTER;
      if( (userocb = TM_master->ucb->directory) < (OCB *) 0x00000001 )
      {


      }
      os_memc( &(TM_master->ucb->directory->oid),
               &(TM_master->ucb->user_id)       , sizeof( OID ) );

      rc = obj_rep( psi_ocb, psi_ocd, & (userocb->oid), SEG_UPDATE );

      gt_name( userocb, S_NULL, FULL_TEIES_NAME,&(TM_master->ucb->name));
      s_init( &name, "%NAME" );
      var_set( name, TM_master->ucb->name    );

      s_init( &laston, "%LASTON" );
      rc = obj_find( userocb, "SYSTEM.STATUS", & user_status );
      if( user_status == FIRST_TIME_USER  ||
          user_status == TM_FIRST_SIGNON    )
      {


         string temp     = S_NULL,
                stat_msg = S_NULL;


         isv_get( & temp, ISV_FIRSTON, 0 );
         s_init(  & stat_msg, "(" );
         s_cat(   & stat_msg, stat_msg, temp, 0, 0 );
         temp = S_NULL;
         s_init(  & temp,     ")" );
         s_cat(   & stat_msg, stat_msg, temp, 0, 0 );
         var_set( laston, stat_msg );

         s_drop ( & temp        );
         s_drop ( & stat_msg    );

      }
      else
      {

         s_init( & CREATED,  "CREATED"  );
         s_init( & MODIFIED, "MODIFIED" );
         s_init( & RECEIVED, "RECEIVED" );

         rc = t_get_last_user_stat_record( & stat_entry );

         if( rc == 0 )
         {

            stat_record = (USER_STAT_ENTRY *)    s_buffer( stat_entry );
            obj_find(  userocb, "SYSTEM.LAST_ON", & last_on_dt  );
            s_dttos( & dt, & last_on_dt, S_NULL                 );
            var_set(   laston, dt                               );
            s_sitos( & convert, stat_record->count_oid_created  );
            var_set(   CREATED, convert                         );
            s_sitos( & convert, stat_record->count_oid_modified );
            var_set(   MODIFIED, convert                        );
            s_sitos( & convert, stat_record->count_oid_received );
            var_set(   RECEIVED, convert                        );
            s_drop(  & convert                                  );

         }
         s_drop(  & CREATED    );
         s_drop(  & MODIFIED   );
         s_drop(  & RECEIVED   );
         s_drop(  & stat_entry );
         ocd_drp( & ocd        );

      }

      s_init( & state, "SIGNON" );
      request = t_form( state, NIL, userocb );

      if( request == USER_REQUESTED_ABANDON )
      {

         rc = SIGNOFF_REQUESTED ;

      }
      else
      {

         rc = 0;

      }
   }
   f_close(  state, STATE_FORM );

}

ocd_drp(& psi_ocd           );
s_drop( & state             );
s_drop( & dtme              );
s_drop( & dt                );
s_drop( & access_mth        );
s_drop( & laston            );
s_drop( & name              );
TM_master->flags &= ~IN_SIGNON;

if( TM_master->usrtype == MASTER_SERVER )
{                                        /******************************/
                                         /*                            */
                                         /* If it is the MS, then wait */
                                         /* for a connection & request */
                                         /* from other servers.        */
   t_syserv();                           /*                            */
   rc = SIGNOFF_REQUESTED;               /*                            */
                                         /******************************/
}

   return(rc);

}
