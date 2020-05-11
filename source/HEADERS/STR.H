/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     STR H                                                  |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  String functions header file for external use; this    |*/
/*|            file will only:                                        |*/
/*|                                                                   |*/
/*|               (1) #include str_x h, the actual external header    |*/
/*|                   file;                                           |*/
/*|                                                                   |*/
/*|               (2) #include str_comm h, the header file of         |*/
/*|                   definations common to internal and external     |*/
/*|                   header files if it was not already included;    |*/
/*|                   this is done so that:                           |*/
/*|                                                                   |*/
/*|                   (a) definations for both sets can be updated    |*/
/*|                       from a central point;                       |*/
/*|                                                                   |*/
/*|                   (b) both str_i h and str h can be #included     |*/
/*|                       in the same source without conflict;        |*/
/*|                                                                   |*/
/*|               (3) make sure that files that str_x.h is            |*/
/*|                   dependant on are included                       |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.    #include's str_x.h, str_comm.h                         |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     23_Mar_87   K.J.Walsh   Written.                       |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized / Trimmed MODS.   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef STR_H_INCLUDED
#  define STR_H_INCLUDED

#  ifndef DEFS_H_INCLUDED
#     error Defs.h required by str.h
#  endif

#  include "str_comm.h"
#  include "str_x.h"

#endif
