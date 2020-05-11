/*+-------------------------------------------------------------------+
  |  <DOC>                                                            |
  |  <NAME>    OBJ H                                                  |
  |  <AUTHOR>  JF                                                     |
  |  <DATE>    February 16, 1989                                      |
  |  <LEVEL>   OBJ                                                    |
  |  <PURPOSE>  -- Checks if DEFS.H which OBJ headers are dependent   |
  |                on are included.                                   |
  |             -- Includes the OBJ_X header file                     |
  |             -- Includes the OBJ_COMM header file if it has not    |
  |                been included yet                                  |
  |             -- Includes the OBJ_ERRS header file                  |
  |  </Doc>                                                           |
  +-------------------------------------------------------------------+*/

#ifndef OBJ_H_INCLUDED

#define OBJ_H_INCLUDED 1

#ifndef DMS_H_INCLUDED
#ifndef DMS_I_H_INCLUDED
#include "dms.h"
#endif
#endif


#ifndef DATAPRIM_H_INCLUDED
#include "dataprim.h"                                 /* $MAKER-IGNORE */
#endif


#ifndef PRIV_H_INCLUDED
#include "priv.h"
#endif


#include "obj_comm.h"
#include "obj_x.h"
#include "obj_errs.h"                                 /* $MAKER-IGNORE */


#endif
