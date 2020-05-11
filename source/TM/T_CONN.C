/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_conn.c                                               |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    November 29, 1987                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  TM_master                                              |
  |                                                                   |
  |  :PARAM.   server      string    INPUT   Server to find in list.  |
  |                                                                   |
  |  :PURPOSE. Return the pointer to the connection list element      |
  |            corresponding to the server requested.                 |
  |                                                                   |
  |  :OUTPUT.  Return 0 if cannot find.                               |
  |            ISCF_TAB * otherwise.                                  |
  |                                                                   |
  |  :CALLS.   s_comp()                                               |
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


#define   NO_CONNECTIONS_IN_TABLE   (ISCF_TAB *) 0xFFFFFFFF
#define   FIND_LAST_ENTRY           (string    ) 0xFFFFFFFF


ISCF_TAB      * t_conn( server )

string                  server;


{

ISCF_TAB     *current       = NIL ;
sint          rc            =   0 ;




current = TM_master->conn_tab;

if( current == NIL )
{

   current = NO_CONNECTIONS_IN_TABLE;

}
else
{

   if( (server == S_NULL) || (server == FIND_LAST_ENTRY) )
   {

      while( current->next )
      {

         current = current->next;

      }

   }
   else if( current != NIL )
   {

      do
      {

          if( 0 == s_comp( current->serv_name, server, 0, 0, 'M', &rc ) )
          {

             break;

          }
          current = current->next;

      }
      while(  current != NIL );

   }
}

   return(current);

}
