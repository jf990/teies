/********************************************************************
<NAME>             T_MS_ON.C
<LEVEL>            TM
<AUTHOR>           John L. Foster
<DATE>             November 29, 1987

<PARAM>            who       string    INPUT    Server to signon.
<PARAM>            data      string    INPUT    Special signon instructio
<PARAM>            type_of_signon hwordINPUT    See tm_comm.h

<OUTPUT>           sint
<PURPOSE>          Signon request by the server.  Lookup the server id
                   in the local operating system id index to determine
                   if this is a valid user, and if so return the OID of
                   their user object.  If not, send back a failure conf-
                   irmation.

                   If the data string is S_NULL, then signon is derived
                   from the network path information.  Otherwise the
                   data string contains a string with the name followed
                   by a string with the associated password.

                   THE FOLLOWING CONDITIONS MUST BE MET BY A VALID SIGNON:

                   1) User has an entry in the SNI under either

                      a) System ID, or
                      b) TEIES user name, number, or nickname.

                   2) the SNI entry(s) point to a valid TEIES object that
                      is a user type;

                   3) the user object is properly sent back to the
                      requesting server;

                   4) the requesting server has a connection table entry
                      for the Master Server;

                   5) the Master Server has a connection table entry for
                      the user with the correct OID.

<CALLS>            s_init(), obj_opn(), t_sysndx(), ocd_new(), obj_get(),
                   s_move(), s_smp_ul(), sizeof(), t_cls_t().

<ERRORS>
                   System_ID_Not_In_SNI
                   Name_Not_in_SNI
                   Password_Does_Not_Match_Name
                   Improper_Terminal_For_Signon
                   Not_A_TEIES_User_ID

<LIMITS>           For Phase I, if the user does not exist we will
                   allow them to create their own account. What the heck.
</DOC>
 ********************************************************************/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "exc.h"
#include "obj.h"
#include "tm.h"



sint     t_ms_on( who, data, type_of_signon )

string            who;
string            data;
hword             type_of_signon;


{


sint      rc               =      0,
          state            =      0;
hword     flags            =      0,
          command          =      0;
boolean   signon_ok        =  FALSE,
          not_ok_to_link   =  FALSE;
OID      *oid              =    NIL;
OCB      *ocb              =    NIL,
         *ucb              =    NIL;
OCD      *ocd              =    NIL;
string    request          = S_NULL,
          name             = S_NULL,
          password         = S_NULL,
          server           = S_NULL;
ISCF_TAB *conn_table_entry =    NIL;





s_copy( &server, who, 0, 0 );
s_trim( &server, 'B'       );

if( type_of_signon == SIGNON_VERIFY )
{

   name = (string)s_buffer( data );
   password = (string) ( s_buffer( name ) + s_allen( name ) );
   oid = t_getnam( name, ANY_NAME );
   if( oid == NIL )
   {

      rc = Not_A_TEIES_User_ID;
      signon_ok = FALSE;

   }
   else
   {

      signon_ok = t_ckpswd( oid, password );
      if( signon_ok == FALSE )
      {

         rc = Password_Does_Not_Match_Name;

      }
      else
      {

         t_iscf_t( server, REAL_USER );
         t_start(  USER_PROCESS      );

      }
   }
}
else
{
                                                    /*******************/
   oid = t_getnam( server, SYSTEM_NAME );           /*We just asked MS */
   if( OID_POINTER_OK( oid ) )
   {

      signon_ok = TRUE;                             /*to signon.  If MS*/

   }
}                                                   /*finds us in its  */
if( signon_ok )                                     /*user directory he*/
{                                                   /*will either a)   */
                                                    /*send back a confm*/
                                                    /*with the OID of  */
                                                    /*our directory, b)*/
                                                    /*                 */
                                                    /*                 */
                                                    /*send back the    */
                                                    /*directory        */
   ucb = obj_opn(  oid, OBJ_UPDATE  );              /*object, or c)    */
                                                    /*                 */
   if( OCB_POINTER_OK( ucb ) )                      /*send back some of*/
   {                                                /*the dir OCD's.   */
                                                    /*******************/
      conn_table_entry = t_conn ( server );
      if( ! ( ISF_TAB_POINTER_OK( conn_table_entry ) ) )
      {

         rc = t_iscf_t( server, REAL_USER );
         conn_table_entry = t_conn ( server );

      }
      if( ISF_TAB_POINTER_OK( conn_table_entry )  )
      {
          /*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*\
          | Update the connection table so it has a cache of the user's |
          | object ID for reference to this user for privileges, etc.,  |
          \*= = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/

         os_memc(&(ucb->oid),&(conn_table_entry->objectID),sizeof(OID));
         conn_table_entry->conn_type = REAL_USER;

      }
      else
      {

         except( ERROR_IN_TM_CONNECTION_TABLE, FATAL_ERROR,
                                server, S_NULL, S_NULL, S_NULL, S_NULL );

      }
      rc = t_updcnt( ucb );

   }
   else
   {

      rc = except( OID_NOT_IN_SOIDI, CONTROL_ERROR, S_NUL, S_NUL, S_NUL,
                                                    S_NUL, S_NUL       );
      flags = 16;

   }
}
else
{

   flags = ERROR_IN_TRANSACTION;
   if( rc == 0 )
   {

      rc = System_ID_Not_In_SNI;

   }
}
if( flags != 0 )
{

   t_csend( server, rc, S_NULL );

}
else
{

   rc = t_m_obj( server, ucb, NIL, not_ok_to_link, state );

}
if( flags != 0 )
{

          /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\
          | The signon has not been verified so do not allow the      |
          | connection to exist any longer.  A new connection and     |
          | signon attempt are required to continue.                  |
          \- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


   if( ISF_TAB_POINTER_OK( t_conn( server ) ) )
   {

      rc = t_stop( (t_conn( server ) )->tm_pid );

   }
   t_cls_t( server, KEEP_CONNECTION );

}


if( oid != NIL )
{

   os_memd( oid );

}
s_drop( & server  );




   return(rc);

}
