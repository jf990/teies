/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  :NAME.    var_init.c                                             |
  |  :AUTHOR.  John L. Foster                                         |
  |  :DATE.    November 20, 1987                                      |
  |  :LEVEL.   VAR                                                    |
  |  :GLOBAL.  TM_master->current->vars                               |
  |                                                                   |
  |  :PARAM.   name    string *  INOUT    Name of the string variable.|
  |  :PARAM.   s_name  byte   *  INPUT    C String to name the var.   |
  |  :PARAM.   content byte   *  INPUT    String of variable content  |
  |  :PARAM.   type    sint      INPUT    Is contents C str or TEIES? |
  |                                                                   |
  |  :PURPOSE. Initialize a TM variable to a C string value.          |
  |                                                                   |
  |  :OUTPUT.  sint                                                   |
  |  :CALLS.   var_set()                                              |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "vars.h"


sint   var_init( name, s_name,  content, type )

string         *name;
byte         *s_name;
void       *content ;
sint            type;

{

string   data      = S_NULL;
boolean  allocated =  FALSE;




if( (type < OC_LOWER_LIMIT) && (type > OC_UPPER_LIMIT) )
{


}
else
{

   s_init(    name, s_name );
   s_upper( * name, 0, 0   );


   if( type == OC_CSTR )
   {

      allocated = TRUE;
      s_init( & data, content );

   }
   else if ( type == OC_STR )
   {

      data = (string) content;

   }
   else
   {


   }

   var_set( * name, data );

   if( allocated )
   {

      s_drop( & data );

   }
}


   return(0);

}
