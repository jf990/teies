/*+--------------------------------------------------------------------+
  |                                                                    |
  |      :NAME.         t_disp_t.c                                     |
  |                                                                    |
  |      :AUTHOR.       Anoop K. Srivastava                            |
  |                                                                    |
  |      :DATE.         January 21, 1988.                              |
  |                                                                    |
  |      :LEVEL.        TM                                             |
  |                                                                    |
  |      :GLOBAL.       None.                                          |
  |                                                                    |
  |      :PARAM.        None.                                          |
  |                                                                    |
  |      :PURPOSE.      To display the list of active TM level         |
  |                     connections.                                   |
  |                                                                    |
  |      :OUTPUT.       List of serv_name, method, date-time of all    |
  |                     the connections at TM level.                   |
  |                                                                    |
  |      :CALLS.        t_conn().                                      |
  |                                                                    |
  |      :ERRORS.       None.                                          |
  |                                                                    |
  |      :LIMITS.       None known.                                    |
  |                                                                    |
  |      :END.                                                         |
  |                                                                    |
  +--------------------------------------------------------------------*/

#include"defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "isf.h"
#include "tm.h"

#define END_OF_CONNECTION_TABLE     (ISCF_TAB *) 0xFFFFFFFF


sint     t_disp_t()
{
    ISCF_TAB      *last      = NIL,
                  *t_pointer = NIL;






    last = t_conn(S_NULL);

    if (last == END_OF_CONNECTION_TABLE )
    {
    }
    else if ( last == 0 )
    {
    }
    else
    {
         t_pointer = TM_master->conn_tab ;

         while ( t_pointer != NIL )
         {

              t_pointer = t_pointer->next;


         }
    }


   return(0);
}
