/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_exobj()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    October 25, 1989                                       |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   list_ocb    OCB **     INOUT  List object.             |
  |  <PARAM>   object_type hword      INPUT  type of object for search|
  |  <PARAM>   item_ids    string     INPUT  items to verify          |
  |  <PARAM>   flags       hword      INPUT  procedure variations     |
  |  <PARAM>   bad_items   string *   OUTPUT Items that do not match  |
  |                                                                   |
  |  <PURPOSE> Validate a set of item ids for a given object type and |
  |            return a set of respective OIDs in a list object.  If  |
  |            any ids in the input string are not validated they are |
  |            returned in the output string bad_items.               |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"
#include "obj_list.h"



sint   t_exobj(  list_object, object_type, item_ids, flags, unknown_ids )

OCB           ** list_object;
hword            object_type;
string           item_ids;
hword            flags;
string         * unknown_ids;


{

   sint        rc         =      0;





if( object_type < ANY_OBJ  ||  object_type > MAX_OBJ )
{

   rc = IMPROPER_OBJECT_TYPE;

}
else if( s_len( item_ids ) < 1 )
{

   rc = NO_DATA_PROVIDED;

}
else if( list_object == NIL || list_object == (OCB **) -1 )
{

   rc = INVALID_OCB;

}
else
{

   if( ! OCB_POINTER_OK( * list_object ) )
   {

      rc = cr_list( NIL, S_NULL, 0, list_object );

   }

   rc = t_jmpst( object_type, EXIST_ACT, flags, item_ids, *list_object,
                                                           unknown_ids );

}


   return(rc);

}
