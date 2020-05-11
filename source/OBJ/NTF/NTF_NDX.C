/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    ntf_ndx()                                              |
  |  <AUTHOR>  John L. Foster                                         |
  |  <DATE>    April 2, 1990                                          |
  |  <LEVEL>   OBJ / NOTIFICATIONS                                    |
  |  <PURPOSE> Create the Notification Index.  Opens the system file  |
  |            * * TEIES NTFDATA as the input data of notifications   |
  |            and fills in the System Notification Index (SNTFI).    |
  |                                                                   |
  |            The key for each entry if the notification #define     |
  |            constant found in OBJ_NTF.H.  The data entry is the    |
  |            record found in the input file.                        |
  |                                                                   |
  |            The index must be cleared each time this routine is    |
  |            run in order to rebuild the entries and avoid possible |
  |            anomolies.                                             |
  |                                                                   |
  |  <OUTPUT>  sint                                                   |
  |  <CALLS>   obj_opn(), obj_get(), obj_rep(), obj_rmv().            |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/
#include "defs.h"
#include "debug.h"
#include "str.h"
#include "osk.h"
#include "obj.h"
#include "tm.h"




sint   ntf_ndx( )


{

sint        rc         =      0;
OCB         sntfi      =    NIL,
            input_file =    NIL;
OCD         component  =    NIL;





   return(rc);

}
