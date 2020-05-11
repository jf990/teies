/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     STR_I_I H                                              |*/
/*|                                                                   |*/
/*| :LEVEL.    STR                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Actual internal header file for use inside STR         |*/
/*|            routines; included by STR_I.H.                         |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     06_Aug_86   N.Voronka   Implemented.                   |*/
/*|                                                                   |*/
/*| :MODS.     15_Feb_89   K.J.Walsh   Standardized / Trimmed MODS.   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef STR_I_I_H_INCLUDED
#define STR_I_I_H_INCLUDED

/*+-------------------------------------------------------------------+*/

/*|                                                                   |*/
/*|  It so happens that the "OSLIBE H" header file is dependant on    |*/
/*|  the #inclusion of this header file, so I'm gonna make sure there |*/
/*|  are fire works by telling it that this header was included only  |*/
/*|  if DEFS H was...                                                 |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define S_EXTERN_H 1

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The true allocation length of a string is 4 bytes greater than   |*/
/*|  the amount asked for.  We hide this incase it gets changed.      |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_true_al(int)    (int+S_OVERHEAD)   /* true allocation length */

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  Following is the number that indicates the lowest  value an STR  |*/
/*|  usage allocation field can containt WITHOUT being a simple  one. |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define S_HI_ON           32768
#define S_HI_OFF          32767

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  The following macros set and check (respectively) the high bit   |*/
/*|  in a half-word (TEIES type hword) -- a 16-bit integer.           |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#define s_sthibt(hwrd)    (hwrd + S_HI_ON)
#define s_ckhibt(hwrd)    (hwrd > S_HI_OFF)

/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*|  String optimization--if STR_LEVEL_OPTIMIZED is TRUE, ASSEMBLER   |*/
/*|  copy routines are used.                                          |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/
#ifndef STR_LEVEL_OPTIMIZED
#   define STR_LEVEL_OPTIMIZED (1==1)
#endif

#endif
