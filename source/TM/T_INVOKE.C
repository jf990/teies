/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_invoker()                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 12, 1988                                        |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   name        type      OUTPUT  description              |
  |                                                                   |
  |  <PURPOSE> Return the TM connection ID of the user who is current-|
  |            ly running.  If this is a user server it will return   |
  |            the user.  If a MS or DBS then the user who we are     |
  |            running a process on behalf of.                        |
  |                                                                   |
  |  <OUTPUT>  string                                                 |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "tm.h"



string     t_invoker()


{


   tmentry *process  =      NIL;
   string   invoker  =   S_NULL;



   if( TM_master != NIL && TM_master->current != NIL &&
                           TM_master->current->conn_entry != NIL )
   {

      invoker = TM_master->current->conn_entry->serv_name;

   }
   else
   {


   }

   return(invoker);

}
