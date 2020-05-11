/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_do()                                                 |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    November 29, 1987                                      |
  |  <LEVEL>   TM                                                     |
  |  <GLOBAL>  TM_master   TMCB *                                     |
  |                                                                   |
  |  <PARAM>   server      string    INPUT   Server to exececute for. |
  |                                                                   |
  |  <PARAM>   command     hword     INPUT   TM command requested.    |
  |                                                                   |
  |  <PARAM>   flags       hword     INPUT   Request flags.           |
  |                                                                   |
  |  <PARAM>   data        string    INOUT   String of data structured|
  |                                          according to command.    |
  |                                                                   |
  |  <PARAM>   state       sint      INPUT   State ID to pass along   |
  |                                          for server-server control|
  |                                                                   |
  |  <PARAM>   result      void **   INOUT   Pointer to hold any re-  |
  |                                          sult if needed.          |
  |                                                                   |
  |  <PURPOSE> Invoke a TM/Object command on this server on behalf    |
  |            of another server.  The data string is dependent on the|
  |            command requested. Commands are currently:             |
  |                                                                   |
  |            CONFIRMATION: The target server has finished processing|
  |              a request sent to it by this server.  The resulting  |
  |              confirmation can be "all OK" indicating the command  |
  |              ran to completion, or "Error" where the data of the  |
  |              confirmation is the exception handler error code     |
  |              indicating where the command failed.                 |
  |                                                                   |
  |            SIGNON:  A server is attempting to identifiy itself to |
  |              this server for a signon request.                    |
  |                                                                   |
  |            SIGNON_VERIFY: A server is sending a name and a passwrd|
  |              for signon verification.  Do not imply identity from |
  |              connection data but from signon packet.              |
  |                                                                   |
  |            SIGNOFF: A server is telling this server it no longer  |
  |              needs to be connected to this server.                |
  |                                                                   |
  |            MS_SHUT_DOWN:  Another server is requesting this server|
  |              to shut down and terminate its running ot TEIES.     |
  |                                                                   |
  |            GET_OBJECT:  A server is requesting a specific object  |
  |              that this server may have in its database.  If found,|
  |              the entire object will be sent to the requesting     |
  |              server in a grouping of packed segments.  It is the  |
  |              responsibility of the receiving server to unpack and |
  |              understand the object.                               |
  |                                                                   |
  |            SEND_OBJECT:  A server is going to send an entire      |
  |              object to this server in the packed segment format.  |
  |                                                                   |
  |            OCD_REQUEST:  A server is requesting one or more Object|
  |              Components of a particular object that this server   |
  |              may be responsible for.  The OID for the object, the |
  |              number of OCD's, and the OCD and respective contents |
  |              are all in the data packet.                          |
  |                                                                   |
  |            OCD_SEND:  A server wants to send some OCD's to this   |
  |              server in the same format mentioned in OCD_REQUEST.  |
  |                                                                   |
  |            TM_ON:  A server would like a copy of the connection   |
  |              table on this server.                                |
  |                                                                   |
  |            OBJECT_ACTION:  A sever is asking this server to       |
  |              perform an object/action routine on its behalf.  The |
  |              object and action code are 2 half words in the data  |
  |              packet, followed by an OID.                          |
  |                                                                   |
  |            LOCATE_IN_SYSTEM_INDEX:  A server is asking to get a   |
  |              component of data that is in a system index.  The    |
  |              confirmation send routine is called to send back any |
  |              error code and/or the resulting data.  Packet format:|
  |                                                                   |
  |                 +--------------+-------------+                    |
  |                 | SYS_OBJ_TYPE |  OCD_PACKET |                    |
  |                 | 2 bytes      |   string    |                    |
  |                 +--------------+-------------+                    |
  |                                                                   |
  |            UPDATE_SYSTEM_INDEX:  A server is asking this server to|
  |              take an OCD and respective data and update a given   |
  |              system index object.  Same packet format as used for |
  |              locate in system index only with the data added on.  |
  |                                                                   |
  |                 +--------------+-------------+--------+           |
  |                 | SYS_OBJ_TYPE |  OCD_PACKET |  Data  |           |
  |                 | 2 bytes      |   string    | String |           |
  |                 +--------------+-------------+--------+           |
  |                                                                   |
  |            SAY_SOMETHING:  It is questionable that the requested  |
  |              server is operating, so give it something trivial to |
  |              to and see if it responds.                           |
  |                                                                   |
  |            SEND_SEGMENT:  A server is requesting a set of components
  |              of a particular object to be sent back in dms_pak()  |
  |              format.  The OID and the component string are required.
  |                                                                   |
  |            RECEIVE_SEGMENT:  Converse to sed_segment, a server wants
  |              to send us the data associated to a particular segment
  |              in an object.  The packet contains the OID and dms_unpk
  |              formatted data.                                      |
  |                                                                   |
  |            RECEIVE_OBJECT_ID:  A server wants to ask another server
  |              for the Item ID of an object given its object ID (OID).
  |                                                                   |
  |            SEND_OBJECT_ID:  A request from another server has come
  |              asking for am item ID from this server.              |
  |                                                                   |
  |            SEND_SINGLE_SEGMENT : A more efficient version of SEND_|
  |              segment where an object ID, component pack and segment
  |              are passed from one server and replace in the respective
  |              object in another server.  No confirmation is generated.
  |                                                                   |
  |            SESION_STAT_INFO :  Get the USER_STAT_ENTRY of the cur-|
  |              rent signed on session from the server.  The struct  |
  |              is defined in OBJ_USER.h and maintained in t_ms_on and
  |              t_ms_off.                                            |
  |                                                                   |
  |              NOTE: For all the above functions see the respective |
  |              source code documentation for the format of the data |
  |              packet as it is different for each command.          |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>   isf_recv(), os_memc(), os_mema(), s_init(),            |
  |            t_m_on(), t_ms_on(), t_ms_off(), t_m_obj(), t_s_obj(), |
  |            t_m_ocd(), t_s_ocd(), t_confirm(), t_active(), t_ms_off|
  |                                                                   |
  |  <ERRORS>  - see OUTPUT above -                                   |
  |            28109 : Command requested is not known.                |
  |            28075 : Server out of service.                         |
  |            28076 : Invalid request, service not done.             |
  |            28077 : Server busy.                                   |
  |            28078 : Nothing to receive from Server requested.      |
  |                                                                   |
  |  <MODS>    Dec_14_88    JF    Added SAY_SOMETHING and UPDATE_USER_|
  |                               NEW_INDEX commands.                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "isf.h"
#include "exc.h"
#include "tm.h"



sint   t_do  ( server, command, flags, state, data, result )

string         server;
hword                  command;
hword                           flags;
sint                                   state;
string                                        data;
void                                              * result;


{

string        request          = S_NULL,
              result_data      = S_NULL,
              return_buffer    = S_NULL,
              receive_buffer   = S_NULL;
sint          rc               = 0,
              process_id       = 0,
              level            = 0;
OCB         * ocb              = NIL;
ISCF_TAB    * conn_table_entry = NIL;




   conn_table_entry = t_conn( server );

   if(    ( (command != SIGNON) && (command != SIGNON_VERIFY) )    )
   {

      if( ! ISF_TAB_POINTER_OK(conn_table_entry ))
      {


#if 0
         rc = except( USER_NOT_SIGNED_ON, CONTROL_ERROR, server,
                                         S_NULL, S_NULL, S_NULL, S_NULL);
#endif

      }
      else
      {

         process_id = conn_table_entry->tm_pid;

      }
   }
   else                                    /* User IS trying to signon */
   {

      if( ISF_TAB_POINTER_OK( conn_table_entry ) )
      {

         if( conn_table_entry->conn_type != PENDING_SIGNON )
         {


         }
         process_id = conn_table_entry->tm_pid;

      }
      else
      {

         process_id = 1 ;

      }
   }

#if 0

   if( rc == 0 )
   {

#endif

      t_active( process_id );

      switch( command )
      {

        case SIGNON      :
                           rc = t_ms_on( server, data, SIGNON );
                           break;
        case SIGNOFF     :
                           rc = t_ms_off( server, SIGNOFF );
                           break;
        case GET_OBJECT  :
                           rc = t_g_obj( server, flags, data );
                           break;
        case SEND_OBJECT :
                           rc = t_s_obj( server, (OCB **) result, flags,
                                                           state, data );
                           break;
        case OCD_REQUEST :
                           rc = t_m_ocd( server, data, (OCB **) result );
                           break;
        case OCD_SEND    :
                           rc = t_s_ocd( server, data, (OCB **) result,
                                                                 flags );
                           break;
        case TM_ON       :
                           rc = t_m_on( server, data );
                           break;
        case MS_SHUT_DOWN:
                           s_init( &return_buffer, "0000"            );
                           s_cat( &return_buffer, return_buffer, data, 0,0);
                           os_memn( s_buffer( return_buffer ), 4     );
                           TM_master->flags |= SHUT_DOWN_REQUESTED;
                           t_send( server, CONFIRMATION, 0, state,
                                                         return_buffer );
                           s_drop( & return_buffer );
                           break;
        case CONFIRMATION:
                           rc = t_cfirm( server, data, flags, result);
                           break;
        case SIGNON_VERIFY:
                           rc = t_ms_on( server, data, SIGNON_VERIFY );
                           break;
        case OBJECT_ACTION:
                           if( s_len( data ) > 16 )
                           {

                              rc =
                              t_jmpst(
                                     *(hword *)( s_buffer( data ) + 0  ),
                                     *(hword *)( s_buffer( data ) + 2  ),
                                     *(hword *)( s_buffer( data ) + 12 ),
                                     (string)  ( s_buffer( data ) + 14 ),
                               ocb = obj_opn((OID *)(s_buffer( data )+4),
                                              OBJ_UPDATE               ),
                                     & result_data
                                   );
                              if( rc == TM_SEND_CONFIRMATION )
                              {

                                 rc = t_csend(server,
                                              TM_master->current->except,
                                              result_data );
                                 TM_master->current->except = 0;

                              }
                              if( result_data != S_NULL )
                              {

                                 s_drop( & result_data );

                              }
                              if( OCB_POINTER_OK( ocb ) )
                              {

                                 rc  = obj_cls( ocb );
                                 ocb = NIL;

                              }
                           }
                           break;
        case UPDATE_SYSTEM_INDEX :
                           rc = t_m_put( server, data );
                           break;
        case LOCATE_IN_SYSTEM_INDEX :
                           rc = t_m_fnd( server, data );
                           break;
        case SAY_SOMETHING          :
                           break;
        case SEND_SEGMENT :
                           rc = t_sndocd( server, data, flags,
                                                     (string *) result );
                           break;
        case RECEIVE_SEGMENT :
                           rc = t_rcvocd( server, data, flags,
                                                     (string *) result );
                           break;
        case RECEIVE_OBJECT_ID :
                           rc = t_objid( server,
                           (OID *) s_buffer( data ), (string *) result );
                           break;
        case SEND_OBJECT_ID :
                           s_copy( (string *) result, data, 0, 0 );
                           break;
        case SEND_SINGLE_SEGMENT :
                           rc = obj_rcvseg( server, data );
                           break;
        case SESSION_STAT_INFO   :
                           rc = t_ms_off( server, SESSION_STAT_INFO );
                           break;
        default          :
                           rc = Unknown_TM_Function;
                           break;

      }


      if( process_id > 1 )
      {

         t_active( TM_GLOBAL_PROCESS_ID );

      }


#if 0
   }
#endif


   return(rc);

}
