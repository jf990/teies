/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_incic                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    January 16, 1990                                       |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   conn_entry   ISCF_TAB * INPUT User's connection table  |
  |                                           entry to modify.        |
  |  <PARAM>   flag         byte       INPUT What attribute to inc-   |
  |                                           rement.                 |
  |  <PURPOSE> Increment either the items modified or items created   |
  |            structure entry in the connection table element re-    |
  |            quested.  If the ISCF_TAB parameter is NIL then do it  |
  |            for the current user (invoker).                        |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <LIMITS>  Only works correctly on the MS.  Will not crash or have|
  |             any adverse affects on any other server, just serves  |
  |             no purpose.                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"




sint   t_incic(  connection_entry, what_to_increment  )

ISCF_TAB       * connection_entry;
byte             what_to_increment;


{

   sint        rc         =      0;
   string      invoker    = S_NULL;



if( connection_entry == NIL )
{

   invoker = t_invoker();
   if( s_len( invoker ) > 0 )
   {

      connection_entry = t_conn(  invoker  );
      if( connection_entry == NIL )
      {
         rc = USER_NOT_SIGNED_ON;
      }
   }
   else
   {
      rc = PROCESS_IS_NOT_ACTIVE;
   }
}

if( connection_entry != NIL )
{

   switch( what_to_increment )
   {

      case INCREMENT_ITEMS_CREATED :

         connection_entry->created ++ ;
         break;

      case INCREMENT_ITEMS_MODIFIED:

         connection_entry->modified ++ ;
         break;

      case INCREMENT_ITEMS_RECEIVED:

         connection_entry->received ++ ;
         break;

      default                      :

         rc = FUNCTION_NOT_SUPPORTED;
         break;

   }
}

   return(rc);

}
