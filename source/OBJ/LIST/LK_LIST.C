/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    lk_list                                                |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1989                                        |
  |  <LEVEL>   OBJ                                                    |
  |  <PARAM>   list_ocb    OCB        INPUT  List to link.            |
  |  <PARAM>   modifier    string     INPUT  List item identifier.    |
  |  <PARAM>   level       hword      INPUT  Type of link to perform. |
  |  <PARAM>   result      OCB **     INPUT  Resulting object.        |
  |                                                                   |
  |  <PURPOSE> Link a list.                                           |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"


sint     lk_list( list_object, modifier, level, result )

OCB          *    list_object;
string            modifier;
hword             level;
void         *    result;

{



   return(0);

}
