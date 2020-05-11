/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ntf_get()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 2, 1990                                          |
  |  <LEVEL>   OBJ / NOTIFICATIONS                                    |
  |  <PARAM>   ntf_entry   sint           INPUT    Notification entry |
  |                                                 to find.          |
  |  <PARAM>   result      string *       OUTPUT   Where to place re- |
  |                                                 sult if we find it|
  |  <PURPOSE> Given an entry return the notification index string.   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   obj_opn(), obj_get().                                  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   ntf_ndx(  sint ntf_entry,  string * result  )


{

sint        rc         =      0;
OCB         sntfi      =    NIL;
OCD         component  =    NIL;





   return(rc);

}
