/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_IfQuit()                                             |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    August 13, 1988                                        |
  |  <LEVEL>   TM                                                     |
  |  <PARAM>   string_to_check  string  INPUT   What to test.         |
  |  <PURPOSE> Check a return value           if it is an abandon or  |
  |            Quit, else return false.                               |
  |  <OUTPUT>  boolean                                                |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "tm.h"
#include "form.h"





boolean  t_ifquit(  value_to_check  )

hword               value_to_check;


{

boolean    rc         =  FALSE;






   if( value_to_check == USER_REQUESTED_ABANDON
   ||  value_to_check == USER_REQUESTED_QUIT
   ||  value_to_check == TM_FORM_NOT_FORMED      )
   {

       rc = TRUE;

   }


   return(rc);

}
