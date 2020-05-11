/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    OBJ_I H  (Internal header file for Object level)       |
  |  <AUTHOR>  Nestor Voronka                                         |
  |  <DATE>    June  8, 1987                                          |
  |  <LEVEL>   Object                                                 |
  |  <PURPOSE>  -- Checks if DEFS.H which OBJ headers are dependent   |
  |                on are included                                    |
  |             -- Includes the OBJ_I_I header file                   |
  |             -- Includes the OBJ_COMM header file if it has not    |
  |                been included yet                                  |
  |  </DOC>                                                           |
  +-------------------------------------------------------------------+*/

#ifndef DMS_H_INCLUDED
#include "dms.h"
#endif


#include "obj_i_i.h"

#ifndef OBJ_COMM
#include "obj_comm.h"
#endif
