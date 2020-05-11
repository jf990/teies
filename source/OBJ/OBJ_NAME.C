/*+-------------------------------------------------------------------+
  |                                                                   |
  |  :NAME.    obj_name.c                                             |
  |                                                                   |
  |  :AUTHOR.  John L. Foster                                         |
  |                                                                   |
  |  :DATE.    June 28, 1988                                          |
  |                                                                   |
  |  :LEVEL.   OBJ                                                    |
  |                                                                   |
  |  :GLOBAL.  refers to the tables created in OBJ_TABL.H             |
  |            objtable()  struct _OBJ_TABLE                          |
  |                                                                   |
  |  :PARAM.   object_type  hword     INPUT   Object type to get name |
  |                                           string of.              |
  |                                                                   |
  |  :PURPOSE. Determine if the type is a valid object, and if so     |
  |            return a string containing its name.                   |
  |                                                                   |
  |  :OUTPUT.  string.  S_NULL if not found.                          |
  |                                                                   |
  |  :CALLS.                                                          |
  |                                                                   |
  |  :ERRORS.  None known.                                            |
  |                                                                   |
  |  :LIMITS.  The resulting string CANNOT be dropped!                |
  |                                                                   |
  |  :END.                                                            |
  |                                                                   |
  +-------------------------------------------------------------------+*/


#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_table.h"






string obj_name( object_type )

hword            object_type;

{

boolean found       = FALSE;
sint    i           =     1,
        rc          =     0;
string  name_to_return = S_NULL;





while(   (! found) && (objtable[ i ].object_name != S_NULL)   )
{

   if( object_type == objtable[ i ].object_type )
   {

      found = TRUE;
      name_to_return = objtable[ i ].object_name;

   }
   ++i;

}


   return(name_to_return);

}
