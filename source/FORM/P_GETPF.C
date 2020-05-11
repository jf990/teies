/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    p_getpf() (Form Get Program Function)                  |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 25, 1989                                        |
  |  <LEVEL>   FORMS                                                  |
  |                                                                   |
  |  <PARAM>   profile_data  p_prof_s *  INPUT   Pointer to form pro- |
  |                                              file to locate cur-  |
  |                                              rent form variables. |
  |                                                                   |
  |  <PARAM>   function      string      INPUT   String containing    |
  |                                              function request to  |
  |                                              convert to integer   |
  |                                              function value.      |
  |  <PURPOSE>                                                        |
  |            Convert a function string that a user or process       |
  |            defined in a given form (p_prof_s) and convert that    |
  |            to a generic function key value.                       |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>                                                          |
  |  <ERRORS>                                                         |
  |  <LIMITS>                                                         |
  |  <MODS>                                                           |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "form_i.h"



sint   p_getpf(   profile_data,  function   )

struct p_prof_s * profile_data;
string                           function;



{

   sint        function_value = 0;





if( profile_data != NIL && s_len( function ) > 0 )
{



}
else
{


   function_value = 0;

}


   return(function_value);

}
