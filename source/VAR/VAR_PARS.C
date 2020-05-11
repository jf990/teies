/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    var_parse.c                                            |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    September 19, 1989                                     |
  |  <LEVEL>   VARS                                                   |
  |  <PARAM>   var_name    string     INPUT  Variable name to parse.  |
  |  <PARAM>   vector      string **  INOUT  Resulting vector of stems|
  |  <PURPOSE> Parse a variable into its individual stem components.  |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   s_parse(), s_copy()                                    |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"




sint   var_parse( var_name, vector )

string            var_name;
string_vector              *vector;


{

   sint        rc         =      0;
   string      extra      = S_NULL,
               v_name     = S_NULL,
               parser     = S_NULL;




if( s_len( var_name ) > 0 && vector != NIL )
{

   s_copy(    & v_name, var_name, 0, 0 );
   s_init(    & parser, "@~,,@n . ," );

   if( * vector == NIL || s_allen( (string) (** vector) ) < 5 )
   {

      s_valloc ( vector, 10 );

   }

   s_parse (  & v_name, parser, S_NULL, * vector, & extra ) ;

   s_drop( & extra  );
   s_drop( & parser );
   s_drop( & v_name );

}
else
{


}



   return(rc);

}
