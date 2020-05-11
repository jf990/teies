/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    t_m_user()                                             |
  |                                                                   |
  |  <AUTHOR>  John L. Foster                                         |
  |                                                                   |
  |  <DATE>    December 14, 1988                                      |
  |                                                                   |
  |  <LEVEL>   TM                                                     |
  |                                                                   |
  |  <GLOBAL>  TM_master                                              |
  |                                                                   |
  |  <PARAM>   server      string     INPUT  Server to update new item|
  |                                          segments.                |
  |                                                                   |
  |  <PARAM>   data        string     INPUT  Data packet.             |
  |                                                                   |
  |  <PARAM>   flags       hword      INPUT  Special request info.    |
  |                                                                   |
  |  <PARAM>   result    **string     INOUT  What to send back.       |
  |                                                                   |
  |  <PURPOSE> Pack the user INDEXES root segment and send it back to |
  |            the requested Server, who will unpack it into the      |
  |            (hopefully) correct user object.  This is to handle any|
  |            updates of new items while the user has been signed on.|
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |                                                                   |
  |  <CALLS>                                                          |
  |                                                                   |
  |  <ERRORS>  None known                                             |
  |                                                                   |
  |  <LIMITS>  None known                                             |
  |                                                                   |
  |  </DOC>                                                           |
  |                                                                   |
  +-------------------------------------------------------------------+*/

#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   t_m_user ( server, data, flags, result )

string            server, data, *result;
hword             flags;


{

   sint        rc         =      0;




   return(rc);

}
