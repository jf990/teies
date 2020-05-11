/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    odt_drop()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    December 13, 1989                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   object_type   hword    INPUT  Object type to drop an   |
  |                                          ODT table for.           |
  |                                                                   |
  |  <PURPOSE> Given the object type, get rid of its Object Definition|
  |            Table.                                                 |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   os_memd()                                              |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   odt_drop( object_type )

hword            object_type;


{

   sint        rc         =      0,
               count      =      0;
   ODT       * object_tab =    NIL;
   ODTLIST   * odt_list   =    NIL;





if( object_type > ANY_OBJ && object_type <= MAX_OBJ )
{

   odt_list = TM_master->odt;

   while( odt_list != NIL && odt_list->odt_ptr != NIL )
   {

      if ( odt_list->obj_type == object_type)
      {

         object_tab = odt_list->odt_ptr;
         break;

      }
      odt_list++;

   }
   if( object_tab != NIL )
   {

      os_memd( odt_list->odt_ptr );
      odt_list->odt_ptr     =  NIL;
      odt_list->obj_type    =    0;

   }
   else
   {

      rc = ODT_DOES_NOT_EXIST;

   }
}
else
{

   rc = UNKNOWN_OBJECT_TYPE;

}


   return(rc);

}
