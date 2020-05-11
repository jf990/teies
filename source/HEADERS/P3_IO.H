/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     P3_IO H                                                |*/
/*|                                                                   |*/
/*| :LEVEL.    PPP                                                    |*/
/*|                                                                   |*/
/*| :GLOBAL.   E_FILE     FILE*      IN   Error file I/O pointer.     |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Provides "extern" for functions which use the error    |*/
/*|            file pointer.                                          |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     23_Jan_87   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef P3_IO_H_INCLUDED
#  define P3_IO_H_INCLUDED
   extern FILE  *E_FILE;           /* Error file pointer            */
#endif
