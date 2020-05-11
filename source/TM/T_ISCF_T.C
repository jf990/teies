/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_iscf_t.c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 29, 1987                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  TM_master                                              |
  |                                                                   |
  |  :PARAM.   server      string    INPUT   Server to add to list.   |
  |                                                                   |
  |  :PARAM.   conn_type   byte      INPUT   TYpe of server connected.|
  |                                                                   |
  |  :PURPOSE. Process a request to connect to this server.  Add to   |
  |            the list of active TM level connections supported over |
  |            ISCF.                                                  |
  |                                                                   |
  |            +--------------+       +                       +       |
  |            |Server Name  *------->|teies string     ...   |       |
  |            +--------------+       +-----------------------+       |
  |            |Access Method*------->|teies string     ...   |       |
  |            +--------------+       +-----------------------+       |
  |            |Status        |                                       |
  |            +--------------+                                       |
  |            |Date/Time On  |                                       |
  |            +--------------+                                       |
  |            |ObjectID      |                                       |
  |            +--------------+       +              +                |
  |            |Next         *------->|Next          |                |
  |            +--------------+       |connection    |                |
  |    <-------*Prev          |       |element       |                |
  |            +--------------+       +--------------+                |
  |            | Conn Type    |                                       |
  |            +--------------+                                       |
  |                                                                   |
  |                                                                   |
  |                                                                   |
  |  :OUTPUT.  Return 0 if transaction received and valid.            |
  |            xx075 : Server out of service.                         |
  |            xx076 : Invalid request, service not done.             |
  |            xx077 : Server busy.                                   |
  |            xx078 : Nothing to receive from Server requested.      |
  |                                                                   |
  |  :CALLS.   isf_recv(), os_memc(), os_mema(), s_init(), isf_qry(). |
  |                                                                   |
  |  :ERRORS.  - see OUTPUT above -                                   |
  |                                                                   |
  |  :LIMITS.  None known.                                            |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "isf.h"
#include "tm.h"



sint   t_iscf_t( server, conn_type )

string           server;
byte                     conn_type;


{

string        method         = S_NULL;
sint          rc             =      0;
ISCF_TAB     *new_connection =    NIL,
             *last_entry     =    NIL;




new_connection = t_conn( server );

if( ! ISF_TAB_POINTER_OK( new_connection ) )
{

   last_entry = new_connection;
   isf_qry( server, QRY_CONNECTION_TYPE, & rc, & method );
   if( ( method == S_NULL ) || ( s_len(method) < 1 ) )
   {

      s_init( & method, "Local" );

   }
   new_connection = (ISCF_TAB *) os_mema( sizeof( ISCF_TAB ));
   os_memn( new_connection, sizeof( ISCF_TAB ) );
   s_copy( &(new_connection->serv_name), server, 0, 0 );
   new_connection->acc_method = method;
   os_dttm( &(new_connection->on) );
   new_connection->conn_type      = conn_type;

   if( last_entry == (ISCF_TAB *) -1 )
   {

      TM_master->conn_tab  = new_connection;
      new_connection->prev = NIL;

   }
   else
   {

      last_entry = t_conn( (string) -1 );
      if( last_entry == NIL )
      {

         os_memd( new_connection );
         rc = ERROR_IN_TM_CONNECTION_TABLE;

      }
      else
      {

         last_entry->next     = new_connection;
         new_connection->prev =     last_entry;

      }
   }
}
else
{

   os_dttm( &(new_connection->on) );

}


   return(rc);

}
