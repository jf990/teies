/*+-------------------------------------------------------------------+*/
/*|                                                                   |*/
/*| :NAME.     STESTVAR H                                             |*/
/*|                                                                   |*/
/*| :LEVEL.    STEST                                                  |*/
/*|                                                                   |*/
/*| :GLOBAL.                                                          |*/
/*|                                                                   |*/
/*| :PARAM.                                                           |*/
/*|                                                                   |*/
/*| :PURPOSE.  Establishes local variables for string test suite.     |*/
/*|                                                                   |*/
/*| :OUTPUT.                                                          |*/
/*|                                                                   |*/
/*| :CALLS.                                                           |*/
/*|                                                                   |*/
/*| :ERRORS.                                                          |*/
/*|                                                                   |*/
/*| :LIMITS.                                                          |*/
/*|                                                                   |*/
/*| :MODS.     02_Aug_88   K.J.Walsh   Implemented.                   |*/
/*|                                                                   |*/
/*| :END.                                                             |*/
/*|                                                                   |*/
/*+-------------------------------------------------------------------+*/

#ifndef STESTVAR_H_INCLUDED

#   define STESTVAR_H_INCLUDED

   string   a = S_NULL;
   string   b = S_NULL;
   string   c = S_NULL;
   string   d = S_NULL;
   string   e = S_NULL;
   string   f = S_NULL;
   string   g = S_NULL;
   string   h = S_NULL;
   string   n = S_NULL;
   string   z = S_NULL;


   sint     rc         = 0;
   sint     rc2        = 0;
   int      tstrc      = 0;
   int      sl         = 0;
   int      testno     = 0;
   int      testset    = 0;
   byte     *c_str     = NIL;
   tint     tintnum    = 0;
   sint     sintnum    = 0;
   hword    hwordnum   = 0;
   real     realnum    = 0;
   byte     bytenum    = '0';
   char     letter     = 'z';

   bool     detail     = FALSE;
   bool     detail2    = FALSE;

#endif
