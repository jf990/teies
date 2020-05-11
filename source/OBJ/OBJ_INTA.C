/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    obj_inta.c                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    June 12, 1988                                          |
  |  <LEVEL>   OBJ                                                    |
  |  <GLOBAL>  refers to the tables created in OBJ_TABL.H             |
  |            objtable()  struct _OBJ_TABLE                          |
  |  <PARAM>   object_name  string    INPUT   Name to verify          |
  |  <PURPOSE> Determine if the name is a valid object.               |
  |  <OUTPUT>  hword                                                  |
  |  <CALLS>   s_comp()                                               |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "str.h"
#include "osk.h"
#include "debug.h"
#include "obj.h"
#include "obj_table.h"






hword  obj_inta( object_name )

string           object_name;

{

hword   object_type =     0;
boolean found       = FALSE;
sint    i           =     1,
        rc          =     0;





s_upper( object_name, 0, 0 );
while(   (! found) && (objtable[ i ].object_name != S_NULL)   )
{

   if                                                                  (
   0 == s_comp( object_name, objtable[ i ].object_name, 0,
                s_len( object_name ), 'M', & rc )
                                                                       )
   {

      found = TRUE;
      object_type = objtable[ i ].object_type;

   }
   ++i;

}
   return(object_type);

}
