/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    t_isfend c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    February 17, 1988                                      |
  |                                                                   |
  |  :LEVEL.   TM                                                     |
  |                                                                   |
  |  :GLOBAL.  None                                                   |
  |                                                                   |
  |  :PARAM.   * none *                                               |
  |                                                                   |
  |  :PURPOSE. Close all existing ISCF connections this server is     |
  |            responsible for.  Once t_conn() returns NIL there are  |
  |            no more entries in the table.                          |
  |                                                                   |
  |  :OUTPUT.  Always return 0.                                       |
  |                                                                   |
  |  :CALLS.   t_conn(), t_cls_t().                                   |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
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
#include "tm.h"
#include "isf.h"



sint   t_isfend()


{

   ISCF_TAB     *isf_entry  =  NIL;





   while(  ( isf_entry = t_conn( S_NULL ))  )
   {

      if( isf_entry != (ISCF_TAB *) 0xFFFFFFFF )
      {

         t_cls_t( isf_entry->serv_name, CLOSE_CONNECTION   );

      }
      else
      {

         break;

      }
   }
   isf_end();

   return(0);

}
